#pragma once

#include "simple_server/connection.h"

class SyncDriveClient{
private:
    Connection* connection;
    std::string user_id;


public:
    SyncDriveClient(Connection* connection);
    ~SyncDriveClient();

    Connection* GetConnection();
    void SetUserId(const std::string& user_id);
    const std::string& GetUserId();

};