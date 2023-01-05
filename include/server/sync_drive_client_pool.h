#pragma once

#include "server/sync_drive_client.h"

class SyncDriveClientPool{
private:
    // use user_id as the key
    std::unordered_map<std::string, std::unique_ptr<SyncDriveClient>> clients;
    std::unordered_map<int, std::unique_ptr<SyncDriveConnection>> connections;
    std::unordered_map<int, SyncDriveClient*> fd_clients_map;

public:
    SyncDriveClientPool();
    ~SyncDriveClientPool();

    void AddConnection(Connection* connection);
    bool RemoveConnection(int fd);
    SyncDriveConnection* GetConnection(int fd);
    SyncDriveClient* GetOrMakeClient(const std::string& user_id);
    SyncDriveClient* GetClient(int fd);
    bool TryDelete(const std::string& user_id);
    bool HasClient(const std::string& user_id);
    bool HasClient(int fd);
    bool HasConnection(int fd);

};