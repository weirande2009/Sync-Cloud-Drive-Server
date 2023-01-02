#pragma once
#include <memory>
#include <functional>
#include <fcntl.h>
#include <utility>

#include "simple_server/common.h"
#include "simple_server/channel.h"
#include "simple_server/socket.h"
#include "simple_server/event_loop.h"

class Acceptor {
public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    explicit Acceptor(EventLoop *loop, const char* ip, int port);
    ~Acceptor();

    int AcceptConnection() const;
    void SetNewConnectionCallback(std::function<void(int)> const &callback);

private:
    std::unique_ptr<Socket> socket;
    std::unique_ptr<Channel> channel;
    std::function<void(int)> new_connection_callback;
};
