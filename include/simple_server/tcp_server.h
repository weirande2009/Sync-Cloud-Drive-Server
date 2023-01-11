#pragma once
#include <functional>
#include <unordered_map>
#include <vector>

#include "simple_server/common.h"
#include "simple_server/event_loop.h"
#include "simple_server/acceptor.h"
#include "simple_server/thread_pool.h"
#include "simple_server/connection.h"
#include "simple_server/reactor_pool.h"
#include "utilities/database.h"

class TcpServer {
public:
    DISALLOW_COPY_AND_MOVE(TcpServer);
    TcpServer(const char* ip, int port);
    ~TcpServer();
    void Start();

    void OnConnect(std::function<void(Connection *)> func);
    void OnDelete(std::function<void(Connection *)> func);
    void OnRecv(std::function<void(Connection *)> func);
    void OnSend(std::function<void(Connection *)> func);

    unsigned int GetReactorNum();

private:
    unsigned int sub_reactor_num;
    std::unique_ptr<EventLoop> main_reactor;
    std::unique_ptr<Acceptor> acceptor;
    std::unique_ptr<ReactorPool> reactor_pool;
    std::unique_ptr<ThreadPool> thread_pool;

    std::function<void(Connection *)> on_connect;
    std::function<void(Connection *)> on_delete;
    std::function<void(Connection *)> on_recv;
    std::function<void(Connection *)> on_send;

protected:
    std::unordered_map<int, std::unique_ptr<Connection>> connections;

    int NewConnection(int fd);
    int DeleteConnection(int fd);
};
