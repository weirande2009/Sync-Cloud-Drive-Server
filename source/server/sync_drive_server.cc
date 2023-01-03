#include "server/sync_drive_server.h"


SyncDriveServer::SyncDriveServer(): TcpServer(){
    // bind process functions
    process_function_pool.push_back(ProcessRegister);
    process_function_pool.push_back(ProcessLogin);
    process_function_pool.push_back(ProcessSync);
    process_function_pool.push_back(ProcessUpload);
    process_function_pool.push_back(ProcessDownLoad);
    process_function_pool.push_back(ProcessUploadData);
    process_function_pool.push_back(ProcessUserChange);
    process_function_pool.push_back(ProcessDeleteFile);
    // bind receive and send callback functions
    OnConnect(std::bind(&SyncDriveServer::SyncDriveNewConnection, this, std::placeholders::_1));
    OnDelete(std::bind(&SyncDriveServer::SyncDriveDeleteConnection, this, std::placeholders::_1));
    OnRecv(std::bind(&SyncDriveServer::ProcessMessage, this, std::placeholders::_1));
    OnSend(std::bind(&SyncDriveServer::SendMessage, this, std::placeholders::_1));
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
int SyncDriveServer::SyncDriveDeleteConnection(Connection* connection){
    clients.erase(connection->GetSocket()->GetFd());
}

/**
 * An overridden function of delete connection
*/
int SyncDriveServer::SyncDriveNewConnection(Connection* connection){
    // make client
    std::unique_ptr<SyncDriveClient> client = std::make_unique<SyncDriveClient>(connection);
    clients[connection->GetSocket()->GetFd()] = std::move(client);
}

/**
 * Process the received data from a connection
*/
void SyncDriveServer::ProcessMessage(Connection* connection){
    TransmissionHeader header;
    memcpy(&header, connection->GetReadBuf()->c_str(), sizeof(TransmissionHeader));
    (this->*process_function_pool[header.cmd-1])(clients[connection->GetSocket()->GetFd()].get());
}

/**
 * Send message to a client where the data has been in the buffer
 * @param connection the client connection
*/
void SyncDriveServer::SendMessage(Connection* connection){
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

void SyncDriveServer::ProcessRegister(SyncDriveClient* client){
    
}

void SyncDriveServer::ProcessLogin(SyncDriveClient* client){

}

void SyncDriveServer::ProcessSync(SyncDriveClient* client){

}

void SyncDriveServer::ProcessUpload(SyncDriveClient* client){

}

void SyncDriveServer::ProcessDownLoad(SyncDriveClient* client){

}

void SyncDriveServer::ProcessUploadData(SyncDriveClient* client){

}

void SyncDriveServer::ProcessUserChange(SyncDriveClient* client){

}

void SyncDriveServer::ProcessDeleteFile(SyncDriveClient* client){

}


