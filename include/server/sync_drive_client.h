#pragma once

#include "simple_server/connection.h"

class SyncDriveClient{
private:
    Connection* connection;



public:
    SyncDriveClient(Connection* connection);
    ~SyncDriveClient();

    Connection* GetConnection();


};