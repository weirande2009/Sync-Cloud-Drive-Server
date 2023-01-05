#pragma once

#include <unordered_set>

#include"server/sync_drive_connection.h"

class SyncDriveClient{
private:
    std::unordered_map<int, SyncDriveConnection*> connections;
    std::unordered_set<int> fds;
    std::string user_id;
    bool file_changed;

public:
    SyncDriveClient();
    SyncDriveClient(const std::string& user_id);
    ~SyncDriveClient();

    void AddConnection(SyncDriveConnection* connection);
    SyncDriveConnection* GetConnection(int fd);
    bool RemoveConnection(int fd);

    void SetUserId(const std::string& user_id);
    const std::string& GetUserId();

    void SetFileChanged(bool file_changed);
    bool GetFileChanged();

    bool Has(int fd);
    bool Empty();

};