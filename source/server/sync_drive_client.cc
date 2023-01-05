#include "server/sync_drive_client.h"

SyncDriveClient::SyncDriveClient(){
    file_changed = false;
}

SyncDriveClient::SyncDriveClient(const std::string& user_id){
    this->user_id = user_id;
    file_changed = false;
}

SyncDriveClient::~SyncDriveClient(){
    
}

void SyncDriveClient::AddConnection(SyncDriveConnection* connection){
    int fd = connection->GetConnection()->GetSocket()->GetFd();
    connections[fd] = connection;
    fds.insert(fd);
}

SyncDriveConnection* SyncDriveClient::GetConnection(int fd){
    if(fds.find(fd) == fds.end()){
        return nullptr;
    }
    return connections[fd];
}

bool SyncDriveClient::RemoveConnection(int fd){
    if(fds.find(fd) == fds.end()){
        return false;
    }
    fds.erase(fd);
    connections.erase(fd);
    return true;
}


void SyncDriveClient::SetFileChanged(bool file_changed){
    this->file_changed = file_changed;
}

bool SyncDriveClient::GetFileChanged(){
    return file_changed;
}

void SyncDriveClient::SetUserId(const std::string& user_id){
    this->user_id = user_id;
}

const std::string& SyncDriveClient::GetUserId(){
    return user_id;
}

bool SyncDriveClient::Has(int fd){
    return fds.find(fd) != fds.end();
}

bool SyncDriveClient::Empty(){
    return connections.empty();
}


