#pragma once
#include <memory>

#include "simple_server/common.h"
#include "simple_server/poller.h"
#include "simple_server/channel.h"

class EventLoop {
public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();


    void Loop() const;
    void UpdateChannel(Channel *channel) const;
    void DeleteChannel(Channel *channel) const;

private:
    std::unique_ptr<Poller> poller;
};
