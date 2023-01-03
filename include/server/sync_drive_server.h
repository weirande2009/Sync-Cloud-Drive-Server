#pragma once

#include <unordered_map>

#include "simple_server/tcp_server.h"
#include "server/sync_drive_client.h"

struct SendPackage{
    int fd;
    int offset;
    int left;
    std::string data;
};

class SyncDriveServer: public TcpServer{
private:
    std::vector<void(SyncDriveServer::*)(SyncDriveClient*)> process_function_pool;
    std::unordered_map<int, std::unique_ptr<SyncDriveClient>> clients;
    
    std::unique_ptr<std::thread> sender;
    std::condition_variable sender_condition_variable;
    std::atomic<bool> sender_stop;
    std::queue<std::unique_ptr<SendPackage>> sender_queue;
    std::mutex sender_queue_mutex;
    void SenderStart();

    int SyncDriveNewConnection(Connection* connection);
    int SyncDriveDeleteConnection(Connection* connection);

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
