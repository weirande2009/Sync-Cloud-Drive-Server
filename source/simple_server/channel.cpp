#include "simple_server/channel.h"

const short Channel::READ_EVENT = 1;
const short Channel::WRITE_EVENT = 2;
const short Channel::ET = 4;

Channel::Channel(int fd, EventLoop *loop) : fd(fd), loop(loop), listen_events(0), ready_events(0), is_exist(false) {

}

Channel::~Channel() { 
    loop->DeleteChannel(this); 
}

void Channel::HandleEvent() const {
    if (ready_events & READ_EVENT) {
        read_callback();
    }
    if (ready_events & WRITE_EVENT) {
        write_callback();
    }
}

void Channel::EnableRead() {
    listen_events |= READ_EVENT;
    loop->UpdateChannel(this);
}

void Channel::EnableWrite() {
    listen_events |= WRITE_EVENT;
    loop->UpdateChannel(this);
}

void Channel::EnableET() {
    listen_events |= ET;
    loop->UpdateChannel(this);
}
int Channel::GetFd() const { 
    return fd; 
}

short Channel::GetListenEvents() const { 
    return listen_events; 
}
short Channel::GetReadyEvents() const { 
    return ready_events; 
}

bool Channel::GetExist() const { 
    return is_exist; 
}

void Channel::SetExist(bool in) { 
    is_exist = in; 
}

void Channel::SetReadyEvent(short ev) {
    if (ev & READ_EVENT) {
        ready_events |= READ_EVENT;
    }
    if (ev & WRITE_EVENT) {
        ready_events |= WRITE_EVENT;
    }
    if (ev & ET) {
        ready_events |= ET;
    }
}

void Channel::SetReadCallback(std::function<void()> const &callback) { 
    read_callback = std::move(callback); 
}

void Channel::SetWriteCallback(std::function<void()> const &callback) { 
    write_callback = std::move(callback); 
}
