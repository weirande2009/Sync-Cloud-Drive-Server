#pragma once

#include <unordered_map>

#include "simple_server/tcp_server.h"
#include "server/sync_drive_client.h"

class SyncDriveServer: public TcpServer{
private:
    std::vector<void(SyncDriveServer::*)(SyncDriveClient*)> process_function_pool;
    std::unordered_map<int, std::unique_ptr<SyncDriveClient>> clients;

    void ProcessMessage(Connection* connection);
    void SendMessage(Connection* connection);

    void ProcessRegister(SyncDriveClient* client);
    void ProcessLogin(SyncDriveClient* client);
    void ProcessSync(SyncDriveClient* client);
    void ProcessUpload(SyncDriveClient* client);
    void ProcessDownLoad(SyncDriveClient* client);
    void ProcessUploadData(SyncDriveClient* client);
    void ProcessUserChange(SyncDriveClient* client);
    void ProcessDeleteFile(SyncDriveClient* client);

public:
    SyncDriveServer();
    ~SyncDriveServer();
    void Start();

};
