#pragma once
#include <functional>
#include <unistd.h>
#include <utility>

#include "simple_server/common.h"
#include "simple_server/event_loop.h"
#include "simple_server/socket.h"

class Channel {
public:
    DISALLOW_COPY_AND_MOVE(Channel);
    Channel(int fd, EventLoop *loop);
    ~Channel();


    void HandleEvent() const;
    void EnableRead();
    void EnableWrite();

    int GetFd() const;
    short GetListenEvents() const;
    short GetReadyEvents() const;
    bool GetExist() const;

    void SetExist(bool in = true);
    void EnableET();
    void SetReadyEvent(short ev);
    void SetReadCallback(std::function<void()> const &callback);
    void SetWriteCallback(std::function<void()> const &callback);

    static const short READ_EVENT;
    static const short WRITE_EVENT;
    static const short ET;

private:
    int fd;
    EventLoop *loop;
    short listen_events;
    short ready_events;
    bool is_exist;
    std::function<void()> read_callback;
    std::function<void()> write_callback;
};
