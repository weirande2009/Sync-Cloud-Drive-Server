#pragma once
#include <vector>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>

#include "simple_server/common.h"
#include "simple_server/channel.h"
#include "simple_server/socket.h"

class Poller {
public:
    DISALLOW_COPY_AND_MOVE(Poller);
    Poller();
    ~Poller();

    int UpdateChannel(Channel *channel) const;
    int DeleteChannel(Channel *channel) const;

    std::vector<Channel *> Poll(int timeout = -1) const;

private:
    int fd;
    struct epoll_event *events{nullptr};
};
