#pragma once

// Macros to disable copying and moving
#define DISALLOW_COPY(cname)     \
    cname(const cname &) = delete; \
    cname &operator=(const cname &) = delete;

#define DISALLOW_MOVE(cname) \
    cname(cname &&) = delete;  \
    cname &operator=(cname &&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname) \
    DISALLOW_COPY(cname);               \
    DISALLOW_MOVE(cname);

struct TransmissionHeader{
    int cmd;
    int length;
};

class Acceptor;
class Buffer;
class Channel;
class Connection;
class EventLoop;
class Poller;
class ReactorPool;
class Socket;
class TcpServer;
class ThreadPool;
