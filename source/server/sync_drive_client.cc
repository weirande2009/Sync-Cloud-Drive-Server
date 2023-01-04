#include "server/sync_drive_client.h"

SyncDriveClient::SyncDriveClient(Connection* connection){
    this->connection = connection;
}

SyncDriveClient::~SyncDriveClient(){
    
}

void SyncDriveClient::SetUserId(const std::string& user_id){
    this->user_id = user_id;
}

const std::string& SyncDriveClient::GetUserId(){
    return user_id;
}

