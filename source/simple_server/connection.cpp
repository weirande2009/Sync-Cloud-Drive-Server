#include "simple_server/connection.h"

Connection::Connection(int fd, EventLoop *loop) {
    sock = std::make_unique<Socket>();
    sock->SetFd(fd);
    if (loop != nullptr) {
        channel = std::make_unique<Channel>(fd, loop);
        channel->EnableRead();
        channel->EnableET();
    }
    read_buf = std::make_unique<Buffer>();
    send_buf = std::make_unique<Buffer>();

    state = State::Connected;
    read_length = sizeof(TransmissionHeader);
    received_length = 0;
}

Connection::~Connection() {

}

int Connection::Recv() {
    if (state != State::Connected) {
        perror("Connection is not connected, can not read");
        return -1;
    }
    return ReadNonBlocking();
}

int Connection::Send(std::string msg) {
    SetSendBuf(msg.c_str());
    if (state != State::Connected) {
        perror("Connection is not connected, can not write");
        return -1;
    }
    on_send(this);
    return 0;
}

int Connection::ReadNonBlocking() {
    int sockfd = sock->GetFd();
    const int read_size = 1024;
    char buf[read_size];
    while (read_length > 0) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, std::min(read_size, read_length));
        if (bytes_read > 0) {
            read_buf->Append(buf, bytes_read);
            read_length -= bytes_read;
            received_length += bytes_read;
        } else if (bytes_read == -1 && errno == EINTR) {  // program normal interrupt, continue reading
            printf("continue reading\n");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // means finishing reading
            break;
        } else if (bytes_read == 0) {  // EOF，disconnected
            printf("read EOF, client fd %d disconnected\n", sockfd);
            return -1;
        } else {
            printf("Other error on client fd %d\n", sockfd);
            return -1;
        }
    }
    return 0;
}

int Connection::WriteNonBlocking() {
    int sockfd = sock->GetFd();
    char buf[send_buf->Size()];
    memcpy(buf, send_buf->c_str(), send_buf->Size());
    int data_size = send_buf->Size();
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytes_write == -1) {
            printf("Other error on client fd %d\n", sockfd);
            state = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
    return 0;
}

void Connection::Business() {
    if(Recv() < 0){  // disconnected
        Close();
        return;
    }
    TransmissionHeader header;
    memcpy(&header, read_buf.get()->c_str(), sizeof(TransmissionHeader));
    // if receive all the data needed
    if(received_length == header.length){
        // process received data
        on_recv(this);
        // Reset receiving length
        read_length = sizeof(TransmissionHeader);
        received_length = 0;
        read_buf.get()->Clear();
        return;
    }
    else if(received_length < header.length){  // finish receiving package head
        // set new read_length
        read_length = header.length - received_length;
    }
    else{
        // something wrong
    }
}

void Connection::SetDeleteConnection(std::function<void(int)> const &func) { 
    delete_connectioin = std::move(func); 
}

void Connection::SetOnRecv(std::function<void(Connection *)> const &func) {
    on_recv = std::move(func);
    std::function<void()> bus = std::bind(&Connection::Business, this);
    channel->SetReadCallback(bus);
}

void Connection::SetOnSend(std::function<void(Connection *)> const &func) {
    on_send = std::move(func);
}

void Connection::SetSendBuf(const char *str) { 
    send_buf->SetBuf(str); 
}

void Connection::Close() { 
    state = State::Closed;
    delete_connectioin(sock->GetFd()); 
}

Connection::State Connection::GetState() const { 
    return state; 
}

Socket *Connection::GetSocket() const { 
    return sock.get(); 
}

Buffer *Connection::GetReadBuf() { 
    return read_buf.get(); 
}

Buffer *Connection::GetSendBuf() { 
    return send_buf.get(); 
}
