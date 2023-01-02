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
    OnRecv(std::bind(&SyncDriveServer::ProcessMessage, this, std::placeholders::_1));
    OnSend(std::bind(&SyncDriveServer::SendMessage, this, std::placeholders::_1));
}

SyncDriveServer::~SyncDriveServer(){

}

void SyncDriveServer::Start(){
    TcpServer::Start();
}


void SyncDriveServer::ProcessMessage(Connection* connection){

}

void SyncDriveServer::SendMessage(Connection* connection){

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


