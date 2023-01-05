#pragma once

#include "simple_server/connection.h"

class SyncDriveConnection{
private:
    Connection* connection;
    bool initialzed;
    bool syncing;

public:
    SyncDriveConnection(Connection* connection);
    ~SyncDriveConnection();

    Connection* GetConnection();
    int GetFd();
    bool GetInitialized();
    bool GetSyncing();

};