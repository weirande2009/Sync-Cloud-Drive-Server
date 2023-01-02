#pragma once
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "simple_server/common.h"

class Socket {
public:
    DISALLOW_COPY_AND_MOVE(Socket);
    Socket();
    ~Socket();
    void SetFd(int fd);
    int GetFd() const;
    std::string GetAddr() const;
    int Create();
    int Bind(const char *ip, uint16_t port) const;
    int Listen() const;
    int Accept(int &client_fd) const;
    int Connect(const char *ip, uint16_t port) const;
    int SetNonBlocking() const;
    bool IsNonBlocking() const;
    size_t RecvBufSize() const;

private:
    int fd;
};
