#include "simple_server/socket.h"


Socket::Socket() : fd(-1) {

}

Socket::~Socket() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::SetFd(int fd) { 
    this->fd = fd; 
}

int Socket::GetFd() const { 
    return fd; 
}

std::string Socket::GetAddr() const {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    if (getpeername(fd, (struct sockaddr *)&addr, &len) == -1) {
        return "";
    }
    std::string ret(inet_ntoa(addr.sin_addr));
    ret += ":";
    ret += std::to_string(htons(addr.sin_port));
    return ret;
}

int Socket::SetNonBlocking() const {
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1) {
        perror("Socket set non-blocking failed");
        return -1;
    }
    return 0;
}

bool Socket::IsNonBlocking() const { 
    return (fcntl(fd, F_GETFL) & O_NONBLOCK) != 0; 
}

size_t Socket::RecvBufSize() const {
    size_t size = -1;
    if (ioctl(fd, FIONREAD, &size) == -1) {
        perror("Socket get recv buf size failed");
    }
    return size;
}

int Socket::Create() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("Failed to create socket");
        return -1;
    }
    return 0;
}

int Socket::Bind(const char *ip, uint16_t port) const {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    if (::bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to bind socket");
        return -1;
    }
    return 0;
}

int Socket::Listen() const {
    if (::listen(fd, SOMAXCONN) == -1) {
        perror("Failed to listen socket");
        return -1;
    }
    return 0;
}

int Socket::Accept(int &client_fd) const {
    // TODO: non-blocking
    client_fd = ::accept(fd, NULL, NULL);
    if (client_fd == -1) {
        perror("Failed to accept socket");
        return -1;
    }
    return 0;
}

int Socket::Connect(const char *ip, uint16_t port) const {
    // TODO: non-blocking
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    if (::connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to connect socket");
        return -1;
    }
    return 0;
}
