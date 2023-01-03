#include "simple_server/event_loop.h"

EventLoop::EventLoop(int no) { 
    poller = std::make_unique<Poller>(); 
    this->no = no;
}

EventLoop::~EventLoop() {

}

void EventLoop::Loop() const {
    while (true) {
        for (Channel *channel : poller->Poll()) {
            channel->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel *channel) const { 
    poller->UpdateChannel(channel); 
}

void EventLoop::DeleteChannel(Channel *channel) const { 
    poller->DeleteChannel(channel); 
}

int EventLoop::GetNo(){
    return no;
}

