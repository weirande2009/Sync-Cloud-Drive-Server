#include "server/sync_drive_server.h"


SyncDriveServer::SyncDriveServer(): TcpServer(){
    // bind process functions
    process_function_pool.push_back(ProcessRegister);
    process_function_pool.push_back(ProcessLogin);
    process_function_pool.push_back(ProcessSync);
    process_function_pool.push_back(ProcessUploadFile);
    process_function_pool.push_back(ProcessDownLoad);
    process_function_pool.push_back(ProcessUploadData);
    process_function_pool.push_back(ProcessDeleteFile);
    process_function_pool.push_back(ProcessAddDirectory);
    process_function_pool.push_back(ProcessDeleteDirectory);
    process_function_pool.push_back(ProcessModifyUser);
    // client pool
    client_pool = std::make_unique<SyncDriveClientPool>();
    // bind receive and send callback functions
    OnConnect(std::bind(&SyncDriveServer::SyncDriveNewConnectionCallback, this, std::placeholders::_1));
    OnDelete(std::bind(&SyncDriveServer::SyncDriveDeleteConnectionCallback, this, std::placeholders::_1));
    OnRecv(std::bind(&SyncDriveServer::ProcessMessageCallback, this, std::placeholders::_1));
    OnSend(std::bind(&SyncDriveServer::SendMessageCallback, this, std::placeholders::_1));
    // start sender
    sender = std::make_unique<std::thread>(SenderStart, this);
    // start databse
    Database::GetInstance()->Start(GetReactorNum());
}

SyncDriveServer::~SyncDriveServer(){

}

void SyncDriveServer::Start(){
    TcpServer::Start();
}

/**
 * The thread function for sending data to clients
*/
void SyncDriveServer::SenderStart(){
    while (true) {
        SendPackage *package;
        {
            std::unique_lock<std::mutex> lock(sender_queue_mutex);
            if(sender_queue.empty()){  // if empty, wait until there is a package
                sender_condition_variable.wait(lock, [this]() { 
                    return sender_stop || !sender_queue.empty(); 
                });
            }
            if (sender_stop) {
                return;
            }
            // get a package from queue
            package = sender_queue.front().get();
        }
        // send the package
        bool fail = false;
        while (package->left > 0) {
            ssize_t bytes_write = write(package->fd, package->data.c_str() + package->offset, package->left);
            if (bytes_write == -1 && errno == EINTR) {
                printf("continue writing\n");
                continue;
            }
            else if (bytes_write == -1 && errno == EAGAIN) {
                break;
            }
            else if (bytes_write == -1) {
                printf("Other error on client fd %d\n", package->fd);
                // close client
                fail = true;
                break;
            }
            package->left -= bytes_write;
            package->offset += bytes_write;
        }
        // if all data are sent or fail, pop
        if(package->left == 0 || fail){
            {
                std::unique_lock<std::mutex> lock(sender_queue_mutex);
                sender_queue.pop();
            }
        }
    }
}

/**
 * An overridden function of delete connection
*/
int SyncDriveServer::SyncDriveDeleteConnectionCallback(Connection* connection){
    int fd = connection->GetSocket()->GetFd();
    auto client = client_pool->GetClient(fd);
    // if there is a client
    if(client){
        // remove the connection
        client_pool->RemoveConnection(fd);
        // try to remove client
        client_pool->TryDelete(client->GetUserId());
    }
    return 0;
}

/**
 * An overridden function of delete connection
*/
int SyncDriveServer::SyncDriveNewConnectionCallback(Connection* connection){
    client_pool->AddConnection(connection);
    return 0;
}

/**
 * Process the received data from a connection
*/
void SyncDriveServer::ProcessMessageCallback(Connection* connection){
    TransmissionHeader header;
    memcpy(&header, connection->GetReadBuf()->c_str(), sizeof(TransmissionHeader));
    auto conn = client_pool->GetConnection(connection->GetSocket()->GetFd());
    // non-client command or login
    if(header.cmd < NON_CLIENT_FUNCTION_NUM || CheckLogin(conn)){
        (this->*process_function_pool[header.cmd-1])(conn);
    }
}

/**
 * Send message to a client where the data has been in the buffer
 * @param connection the client connection
*/
void SyncDriveServer::SendMessageCallback(Connection* connection){
    auto package = std::make_unique<SendPackage>();
    package->fd = connection->GetSocket()->GetFd();
    package->offset = 0;
    package->data = connection->GetSendBuf()->GetBuf();
    package->left = package->data.length();
    {
        std::unique_lock<std::mutex> lock(sender_queue_mutex);
        sender_queue.emplace(std::move(package));
    }
}

void SyncDriveServer::ProcessRegister(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CRegister c_register;
    c_register.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    UserService user_service;
    // Construct protobuf to send
    SyncCloudDrive::SRegister s_register;
    if(user_service.Register(c_register.name(), c_register.password_md5())){
        s_register.set_state(1);
    }
    else{
        s_register.set_state(0);
    }
    // convert protobuf to string
    s_register.SerializeToString(&send_string);
    // send
    connection->GetConnection()->Send(send_string);
}

void SyncDriveServer::ProcessLogin(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CLogin c_login;
    c_login.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    UserService user_service;
    // Construct protobuf to send
    SyncCloudDrive::SLogin s_login;
    if(user_service.Login(c_login.name(), c_login.password_md5())){
        // add id to client
        auto id = user_service.GetUserId(c_login.name());
        if(id){
            // get or make a client
            auto client = client_pool->GetOrMakeClient(id.value());
            // add connection to the client
            client->AddConnection(connection);
            s_login.set_state(1);
        }
        else{
            s_login.set_state(0);
        }
    }
    else{
        s_login.set_state(0);
    }
    // convert protobuf to string
    s_login.SerializeToString(&send_string);
    // send
    connection->GetConnection()->Send(send_string);
}

void SyncDriveServer::ProcessSync(SyncDriveConnection* connection){

}

void SyncDriveServer::ProcessUploadFile(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CUploadFile c_upload_file;
    c_upload_file.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    FileService file_service;
    FileMD5Service filemd5_service;
    // Check whether the file exists in the user's file
    
    
    // Construct protobuf to send
    SyncCloudDrive::SUploadFile s_upload_file;
    // convert protobuf to string
    s_upload_file.SerializeToString(&send_string);
    // send
    connection->GetConnection()->Send(send_string);
}

void SyncDriveServer::ProcessDownLoad(SyncDriveConnection* connection){

}

void SyncDriveServer::ProcessUploadData(SyncDriveConnection* connection){

}

void SyncDriveServer::ProcessDeleteFile(SyncDriveConnection* connection){

}

void SyncDriveServer::ProcessAddDirectory(SyncDriveConnection* connection){

}

void SyncDriveServer::ProcessDeleteDirectory(SyncDriveConnection* connection){

}

void SyncDriveServer::ProcessModifyUser(SyncDriveConnection* connection){

}

bool SyncDriveServer::CheckLogin(SyncDriveConnection* connection){
    return client_pool->HasClient(connection->GetFd());
}

std::string SyncDriveServer::GetProtobufString(const std::string& data){
    std::string protobuf(data.c_str()+sizeof(TransmissionHeader), data.length()-sizeof(TransmissionHeader));
    return protobuf;
}

