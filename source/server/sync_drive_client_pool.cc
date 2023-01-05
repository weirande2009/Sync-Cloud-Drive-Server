#include "server/sync_drive_client_pool.h"

SyncDriveClientPool::SyncDriveClientPool(){

}

SyncDriveClientPool::~SyncDriveClientPool(){

}

bool SyncDriveClientPool::HasClient(const std::string& user_id){
    return clients.find(user_id) != clients.end();
}

bool SyncDriveClientPool::HasClient(int fd){
    return fd_clients_map.find(fd) != fd_clients_map.end();
}

bool SyncDriveClientPool::HasConnection(int fd){
    return connections.find(fd) != connections.end();
}

void SyncDriveClientPool::AddConnection(Connection* connection){
    auto conn = std::make_unique<SyncDriveConnection>(connection);
    int fd = connection->GetSocket()->GetFd();
    connections[fd] = std::move(conn);
}

bool SyncDriveClientPool::RemoveConnection(int fd){
    if(HasConnection(fd)){
        // remove from connections
        connections.erase(fd);
        // remove from client if there is a client who has the connection
        if(HasClient(fd)){
            fd_clients_map[fd]->RemoveConnection(fd);
        }
        return true;
    }
    return false;
}

SyncDriveConnection* SyncDriveClientPool::GetConnection(int fd){
    if(HasConnection(fd)){
        return connections[fd].get();
    }
    return nullptr;
}


SyncDriveClient* SyncDriveClientPool::GetOrMakeClient(const std::string& user_id){
    // Check whether the client exists
    if(!HasClient(user_id)){
        auto client = std::make_unique<SyncDriveClient>(user_id);
        clients[user_id] = std::move(client);
    }
    return clients[user_id].get();
}

SyncDriveClient* SyncDriveClientPool::GetClient(int fd){
    if(HasClient(fd)){
        return fd_clients_map[fd];
    }
    else{
        return nullptr;
    }
}

bool SyncDriveClientPool::TryDelete(const std::string& user_id){
    // Check whether the client exists
    if(HasClient(user_id)){
        if(clients[user_id]->Empty()){
            clients.erase(user_id);
            return true;
        }
    }
    return false;
}



