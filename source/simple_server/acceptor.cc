#include "simple_server/acceptor.h"

Acceptor::Acceptor(EventLoop *loop, const char* ip, int port) {
    socket = std::make_unique<Socket>();
    if(socket->Create() < 0){

    }
    if(socket->Bind(ip, port) < 0){
        
    }
    if(socket->Listen() < 0){
        
    }
    channel = std::make_unique<Channel>(socket.get()->GetFd(), loop);
    std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
    channel->SetReadCallback(cb);
    channel->EnableRead();
}

Acceptor::~Acceptor() {}

int Acceptor::AcceptConnection() const{
    int client_fd = -1;
    if( socket->Accept(client_fd) < 0 ) {
        return -1;
    }
    // set non-block
    fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL) | O_NONBLOCK);
    if (new_connection_callback) {
        new_connection_callback(client_fd);
    }
    return 0;
}

void Acceptor::SetNewConnectionCallback(std::function<void(int)> const &callback) {
    new_connection_callback = std::move(callback);
}
