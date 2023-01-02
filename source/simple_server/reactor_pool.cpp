#include "simple_server/reactor_pool.h"

ReactorPool::ReactorPool(int num){
    for (size_t i = 0; i < num; ++i) {
        std::unique_ptr<EventLoop> sub_reactor = std::make_unique<EventLoop>();
        sub_reactors.push_back(std::move(sub_reactor));
    }
    connection_num = std::vector<int>(num, 0);
}

ReactorPool::~ReactorPool(){

}

int ReactorPool::GetIndexOfAvailabelReactor(){
    int m = INT_MAX;
    int pos = -1;
    for(int i=0; i<connection_num.size(); i++){
        if(connection_num[i] < m){
            m = connection_num[i];
            pos = i;
        }
    }
    return pos;
}

EventLoop* ReactorPool::BindConnectionToReactor(int fd){
    int pos = GetIndexOfAvailabelReactor();
    if(pos == -1){
        return nullptr;
    }
    IncreaseConnection(pos);
    reactor_index_of_connection[fd] = pos;
    return sub_reactors[pos].get();
}

EventLoop* ReactorPool::GetReactorByIndex(int index){
    if(index < 0 || index >= sub_reactors.size()){
        return nullptr;
    }
    return sub_reactors[index].get();
}

int ReactorPool::RemoveConnection(int fd){
    if(reactor_index_of_connection.find(fd) == reactor_index_of_connection.end()){
        return -1;
    }
    DecreaseConnection(reactor_index_of_connection[fd]);
    reactor_index_of_connection.erase(fd);
    return 0;
}

int ReactorPool::GetConnectionNumInReactor(int index){
    if(index < 0 || index >= sub_reactors.size()){
        return -1;
    }
    return connection_num[index];
}

int ReactorPool::GetReactorIndexOfConnection(int fd){
    if(reactor_index_of_connection.find(fd) == reactor_index_of_connection.end()){
        return -1;
    }
    return reactor_index_of_connection[fd];
}

int ReactorPool::IncreaseConnection(int index){
    if(index < 0 || index >= sub_reactors.size()){
        return -1;
    }
    connection_num[index]++;
    return 0;
}

int ReactorPool::DecreaseConnection(int index){
    if(index < 0 || index >= sub_reactors.size()){
        return -1;
    }
    connection_num[index]--;
    return 0;
}



