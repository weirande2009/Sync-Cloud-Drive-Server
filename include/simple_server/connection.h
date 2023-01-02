#pragma once
#include <functional>
#include <unistd.h>

#include "simple_server/common.h"
#include "simple_server/buffer.h"
#include "simple_server/socket.h"
#include "simple_server/event_loop.h"
#include "simple_server/channel.h"

class Connection {
public:
    enum State {
        Invalid = 0,
        Connecting,
        Connected,
        Closed,
    };
    DISALLOW_COPY_AND_MOVE(Connection);
    Connection(int fd, EventLoop *loop);
    ~Connection();

    void SetDeleteConnection(std::function<void(int)> const &func);
    void SetOnRecv(std::function<void(Connection *)> const &func);
    void SetOnSend(std::function<void(Connection *)> const &func);
    State GetState() const;
    Socket *socket() const;
    void SetSendBuf(const char *str);
    Buffer *ReadBuf();
    Buffer *SendBuf();

    int Recv();
    int Write();
    int Send(std::string msg);

    void Close();

private:
    void Business();
    int ReadNonBlocking();
    int WriteNonBlocking();
    int ReadBlocking();
    int WriteBlocking();

    std::unique_ptr<Socket> sock;
    std::unique_ptr<Channel> channel;

    State state;
    std::unique_ptr<Buffer> read_buf;
    std::unique_ptr<Buffer> send_buf;

    std::function<void(int)> delete_connectioin;
    std::function<void(Connection *)> on_recv;
    std::function<void(Connection *)> on_send;

    int read_length;
    int received_length;

};
