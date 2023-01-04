#pragma once

#include <unordered_map>
#include <unordered_set>

#include "simple_server/tcp_server.h"
#include "server/sync_drive_client.h"

#include "service/user_service.h"
#include "service/directory_service.h"
#include "service/filemd5_service.h"
#include "service/file_service.h"
#include "service/transmission_service.h"

#include "protobuf/client_to_server.pb.h"
#include "protobuf/server_to_client.pb.h"

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

    // on transmission variables
    std::unordered_map<std::string, std::unordered_set<int>> on_transmission_slides;  // on_transmission_slides[i][j] means filemd5 of i and slide_no of j
    std::mutex on_transmission_mutex;

    
    // async sender variables
    std::unique_ptr<std::thread> sender;
    std::condition_variable sender_condition_variable;
    std::atomic<bool> sender_stop;
    std::queue<std::unique_ptr<SendPackage>> sender_queue;
    std::mutex sender_queue_mutex;

    // sender thread function
    void SenderStart();

    // callback functions
    int SyncDriveNewConnectionCallback(Connection* connection);
    int SyncDriveDeleteConnectionCallback(Connection* connection);
    void ProcessMessageCallback(Connection* connection);
    void SendMessageCallback(Connection* connection);

    // processor functions
    void ProcessRegister(SyncDriveClient* client);
    void ProcessLogin(SyncDriveClient* client);
    void ProcessSync(SyncDriveClient* client);
    void ProcessUploadFile(SyncDriveClient* client);
    void ProcessDownLoad(SyncDriveClient* client);
    void ProcessUploadData(SyncDriveClient* client);
    void ProcessDeleteFile(SyncDriveClient* client);
    void ProcessAddDirectory(SyncDriveClient* client);
    void ProcessDeleteDirectory(SyncDriveClient* client);
    void ProcessModifyUser(SyncDriveClient* client);

    // utility functions
    std::string GetProtobufString(const std::string& data);

public:
    SyncDriveServer();
    ~SyncDriveServer();
    void Start();

};
