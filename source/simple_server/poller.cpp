#include "simple_server/poller.h"

#define MAX_EVENTS 1000

Poller::Poller() {
    fd = epoll_create1(0);
    if(fd == -1){

    }
    events = new epoll_event[MAX_EVENTS];
    memset(events, 0, sizeof(*events) * MAX_EVENTS);
}

Poller::~Poller() {
    if (fd != -1) {
        close(fd);
    }
    delete[] events;
}

std::vector<Channel *> Poller::Poll(int timeout) const{
    std::vector<Channel *> active_channels;
    int nfds = epoll_wait(fd, events, MAX_EVENTS, timeout);
    if(nfds == -1){
        return {};
    }
    for (int i = 0; i < nfds; ++i) {
        Channel *ch = (Channel *)events[i].data.ptr;
        int ev = events[i].events;
        if (ev & EPOLLIN) {
            ch->SetReadyEvent(Channel::READ_EVENT);
        }
        if (ev & EPOLLOUT) {
            ch->SetReadyEvent(Channel::WRITE_EVENT);
        }
        if (ev & EPOLLET) {
            ch->SetReadyEvent(Channel::ET);
        }
        active_channels.push_back(ch);
    }
    return active_channels;
}

int Poller::UpdateChannel(Channel *channel) const{
    int sockfd = channel->GetFd();
    struct epoll_event ev {};
    ev.data.ptr = channel;
    if (channel->GetListenEvents() & Channel::READ_EVENT) {
        ev.events |= EPOLLIN | EPOLLPRI;
    }
    if (channel->GetListenEvents() & Channel::WRITE_EVENT) {
        ev.events |= EPOLLOUT;
    }
    if (channel->GetListenEvents() & Channel::ET) {
        ev.events |= EPOLLET;
    }
    if (!channel->GetExist()) {
        if(epoll_ctl(fd, EPOLL_CTL_ADD, sockfd, &ev) == -1){
            return -1;
        }
        channel->SetExist();
    } else {
        if(epoll_ctl(fd, EPOLL_CTL_MOD, sockfd, &ev) == -1){
            return -1;
        }
    }
    return 0;
}

int Poller::DeleteChannel(Channel *channel) const{
    int sockfd = channel->GetFd();
    if(epoll_ctl(fd, EPOLL_CTL_DEL, sockfd, nullptr) == -1){
        return -1;
    }
    channel->SetExist(false);
    return 0;
}
