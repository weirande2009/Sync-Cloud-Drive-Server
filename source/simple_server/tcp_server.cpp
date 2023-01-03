#include "simple_server/tcp_server.h"

TcpServer::TcpServer() {
    main_reactor = std::make_unique<EventLoop>(0);
    acceptor = std::make_unique<Acceptor>(main_reactor.get());
    std::function<void(int)> cb = std::bind(&TcpServer::NewConnection, this, std::placeholders::_1);
    acceptor->SetNewConnectionCallback(cb);

    sub_reactor_num = std::thread::hardware_concurrency();
    thread_pool = std::make_unique<ThreadPool>(sub_reactor_num);
    reactor_pool = std::make_unique<ReactorPool>(sub_reactor_num);
}

TcpServer::~TcpServer() {}

void TcpServer::Start() {
    for (size_t i = 0; i < sub_reactor_num; ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::Loop, reactor_pool.get()->GetReactorByIndex(i));
        thread_pool->Add(std::move(sub_loop));
    }
    main_reactor->Loop();
}

int TcpServer::NewConnection(int fd) {
    if(fd == -1){
        return -1;
    }
    EventLoop* reactor = reactor_pool.get()->BindConnectionToReactor(fd);

    std::unique_ptr<Connection> conn = std::make_unique<Connection>(fd, reactor);
    std::function<void(int)> cb = std::bind(&TcpServer::DeleteConnection, this, std::placeholders::_1);
    
    conn->SetDeleteConnection(cb);
    conn->SetOnRecv(on_recv);
    conn->SetOnSend(on_send);

    connections[fd] = std::move(conn);
    if (on_connect) {
        on_connect(connections[fd].get());
    }
    return 0;
}

int TcpServer::DeleteConnection(int fd) {
    auto it = connections.find(fd);
    if( it == connections.end() ){
        return -1;
    }
    if (on_delete) {
        on_delete(connections[fd].get());
    }
    connections.erase(fd);
    reactor_pool->RemoveConnection(fd);
    return 0;
}

void TcpServer::OnConnect(std::function<void(Connection *)> func) { 
    on_connect = std::move(func); 
}

void TcpServer::OnDelete(std::function<void(Connection *)> func) { 
    on_delete = std::move(func); 
}

void TcpServer::OnRecv(std::function<void(Connection *)> func) { 
    on_recv = std::move(func); 
}

void TcpServer::OnSend(std::function<void(Connection *)> func) { 
    on_send = std::move(func); 
}

unsigned int TcpServer::GetReactorNum(){
    return sub_reactor_num+1;
}

