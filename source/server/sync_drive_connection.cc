#include "server/sync_drive_connection.h"

SyncDriveConnection::SyncDriveConnection(Connection* connection){
    this->connection = connection;
    initialzed = false;
    syncing = false;
}

SyncDriveConnection::~SyncDriveConnection(){

}

Connection* SyncDriveConnection::GetConnection(){
    return connection;
}

bool SyncDriveConnection::GetInitialized(){
    return initialzed;
}

bool SyncDriveConnection::GetSyncing(){
    return syncing;
}

int SyncDriveConnection::GetFd(){
    return connection->GetSocket()->GetFd();
}


