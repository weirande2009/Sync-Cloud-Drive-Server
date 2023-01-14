#include "server/sync_drive_server.h"


SyncDriveServer::SyncDriveServer(int port): TcpServer("127.0.0.1", port){
    // bind process functions
    process_function_pool.push_back(&SyncDriveServer::ProcessRegister);
    process_function_pool.push_back(&SyncDriveServer::ProcessLogin);
    process_function_pool.push_back(&SyncDriveServer::ProcessSync);
    process_function_pool.push_back(&SyncDriveServer::ProcessUploadFile);
    process_function_pool.push_back(&SyncDriveServer::ProcessDownLoad);
    process_function_pool.push_back(&SyncDriveServer::ProcessUploadData);
    process_function_pool.push_back(&SyncDriveServer::ProcessDeleteFile);
    process_function_pool.push_back(&SyncDriveServer::ProcessAddDirectory);
    process_function_pool.push_back(&SyncDriveServer::ProcessDeleteDirectory);
    process_function_pool.push_back(&SyncDriveServer::ProcessModifyUser);
    // client pool
    client_pool = std::make_unique<SyncDriveClientPool>();
    // bind receive and send callback functions
    OnConnect(std::bind(&SyncDriveServer::SyncDriveNewConnectionCallback, this, std::placeholders::_1));
    OnDelete(std::bind(&SyncDriveServer::SyncDriveDeleteConnectionCallback, this, std::placeholders::_1));
    OnRecv(std::bind(&SyncDriveServer::ProcessMessageCallback, this, std::placeholders::_1));
    OnSend(std::bind(&SyncDriveServer::SendMessageCallback, this, std::placeholders::_1));
    // start sender
    sender = std::make_unique<std::thread>(&SyncDriveServer::SenderStart, this);
    // start databse
    Database::GetInstance().Start(GetReactorNum());
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
    // record the slide no. that will be sent, default set as -1, i.e. finishing uploading
    int slide_no = -1;
    // Check whether the file exists in the user's file
    auto file = file_service.GetFile(c_upload_file.name(), c_upload_file.parent_id(), c_upload_file.md5());
    if(file){  // if there is the file
        // there are two possibilities: finished or not finished
        if(file.value()->state == 1){  // finished
            // nothing is needed to do, just send back the finished state
            slide_no = -1;
        }
        else if(file.value()->state == 0){  // unfinished
            // need to check which transmission slide is needed and tell the client
            slide_no = 1;
        }
    }
    else{  // if there isn't the file
        // create a file object for the user
        if(!file_service.AddFile(client_pool->GetClient(connection->GetFd())->GetUserId(), c_upload_file.name(), c_upload_file.parent_id(), c_upload_file.md5())){
            // if fail
            return;
        }
        // Check whether there is a filemd5
        auto filemd5 = filemd5_service.GetByMD5(c_upload_file.md5());
        if(filemd5){  // if there is the filemd5
            // check state
            if(filemd5.value()->state == 1){  // if finished
                // nothing is needed to do, just send back the finished state
                slide_no = -1;
            }
            else{
                // need to check which transmission slide is needed and tell the client
                slide_no = 1;
            }
        }
        else{  // if there isn't the filemd5, i.e. a new filemd5
            // create a new filemd5
            if(!filemd5_service.Add(c_upload_file.md5(), c_upload_file.size())){  // if fail
                return;
            }
            // for a new filemd5, the first slide no. is always 0
            slide_no = 0;
        }
    }
    // Check the slide no.
    if(slide_no != -1){                                              
        // need to record slide no.
        auto filemd5 = filemd5_service.GetByMD5(c_upload_file.md5());
        slide_no = AddNextSlideNoInTransmission(filemd5.value()->id);
    }
    // Construct protobuf to send
    SyncCloudDrive::SUploadFile s_upload_file;
    // set data
    s_upload_file.set_no(c_upload_file.no());
    s_upload_file.set_slide_no(slide_no);
    // convert protobuf to string
    s_upload_file.SerializeToString(&send_string);
    // send
    connection->GetConnection()->Send(send_string);
}

void SyncDriveServer::ProcessDownLoad(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CDownLoad c_download;
    c_download.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    FileService file_service;
    FileMD5Service filemd5_service;
    // get file
    auto file = file_service.GetFile(c_download.file_id());
    if(file){
        // get from md5
        auto data = filemd5_service.ReadData(file.value()->md5, c_download.slide_no());
        if(data){
            // Construct protobuf to send
            SyncCloudDrive::SDownload s_download;
            s_download.set_file_id(c_download.file_id());
            s_download.set_slide_no(c_download.slide_no());
            s_download.set_data(data.value());
            // convert protobuf to string
            s_download.SerializeToString(&send_string);
            // send
            connection->GetConnection()->Send(send_string);
        }
    }
    // fail, do something
}

void SyncDriveServer::ProcessUploadData(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CUploadData c_upload_data;
    c_upload_data.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    FileService file_service;
    FileMD5Service filemd5_service;
    TransmissionService transmission_service;
    // write data to disk
    if(!filemd5_service.WriteData(c_upload_data.md5(), c_upload_data.slide_no(), c_upload_data.data())){
        return;
    }
    // remove from on_transmission
    auto filemd5 = filemd5_service.GetByMD5(c_upload_data.md5());
    if(!filemd5){
        return;
    }
    if(!RemoveSlideNoInTransmission(filemd5.value()->id, c_upload_data.slide_no())){
        return;
    }
    // remove from transmission
    if(!transmission_service.Remove(filemd5.value()->id, c_upload_data.slide_no())){
        return;
    }
    // Check the next slide no.
    int slide_no = GetNextSlideNoInTransmission(filemd5.value()->id);
    if(slide_no == -1){  // if not find the next
        {
            std::unique_lock<std::mutex> lock(on_transmission_mutex);
            if(!transmission_service.HasTransmission(filemd5.value()->id)){  // all are received
                // set state of filemd5
                filemd5_service.UpdateState(filemd5.value()->id, 1);
                // set state of all file of filemd5
                file_service.UpdateAllFileState(filemd5.value()->id, 1);
            }
            else{  // there are still other files on transmission
                // do something
            }
        }
    }
    // Construct protobuf to send
    SyncCloudDrive::SUploadFile s_upload_file;
    // set data
    s_upload_file.set_no(c_upload_data.no());
    s_upload_file.set_slide_no(slide_no);
    // convert protobuf to string
    s_upload_file.SerializeToString(&send_string);
    // send
    connection->GetConnection()->Send(send_string);
}

void SyncDriveServer::ProcessDeleteFile(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CDeleteFile c_delete_file;
    c_delete_file.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    FileService file_service;
    FileMD5Service filemd5_service;
    TransmissionService transmission_service;
    // get client
    auto client = client_pool->GetClient(connection->GetFd());
    // remove from file
    if(file_service.RemoveFile(c_delete_file.file_id())){
        // do something for the client

        // Construct protobuf to send
        SyncCloudDrive::SDeleteFile s_delete_file;
        // set data
        s_delete_file.set_state(1);
        // convert protobuf to string
        s_delete_file.SerializeToString(&send_string);
        // send
        connection->GetConnection()->Send(send_string);
    }
}

void SyncDriveServer::ProcessAddDirectory(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CAddDirectory c_add_directory;
    c_add_directory.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    DirectoryService directory_service;
    // get client
    auto client = client_pool->GetClient(connection->GetFd());
    // add directory
    if(directory_service.AddDirectory(c_add_directory.name(), c_add_directory.parent_id(), client->GetUserId())){
        // do something for the client

        // Construct protobuf to send
        SyncCloudDrive::SAddDirectory s_add_directory;
        // set data
        s_add_directory.set_state(1);
        // convert protobuf to string
        s_add_directory.SerializeToString(&send_string);
        // send
        connection->GetConnection()->Send(send_string);
    }
}

void SyncDriveServer::ProcessDeleteDirectory(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CDeleteDirectory c_delete_directory;
    c_delete_directory.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    DirectoryService directory_service;
    // get client
    auto client = client_pool->GetClient(connection->GetFd());
    // delete directory
    if(directory_service.RemoveDirectory(c_delete_directory.directory_id())){
        // do something for the client

        // Construct protobuf to send
        SyncCloudDrive::SDeleteDirectory s_delete_directory;
        // set data
        s_delete_directory.set_state(1);
        // convert protobuf to string
        s_delete_directory.SerializeToString(&send_string);
        // send
        connection->GetConnection()->Send(send_string);
    }
}

void SyncDriveServer::ProcessModifyUser(SyncDriveConnection* connection){
    std::string send_string;
    // Protobuf
    SyncCloudDrive::CModifyUser c_modify_user;
    c_modify_user.ParseFromString(GetProtobufString(connection->GetConnection()->GetReadBuf()->GetBuf()));
    // Service
    UserService user_service;
    // get client
    auto client = client_pool->GetClient(connection->GetFd());
    // Construct protobuf to send
    SyncCloudDrive::SModifyUser s_modify_user;
    s_modify_user.set_state(0);
    // judge modification type
    if(c_modify_user.type() == UserService::ModificationType::Password){
        if(user_service.ChangePassword(client->GetUserId(), c_modify_user.content())){
            // do something for the client
            s_modify_user.set_state(1);
        }
    }
    // convert protobuf to string
    s_modify_user.SerializeToString(&send_string);
    // send
    connection->GetConnection()->Send(send_string);
}

int SyncDriveServer::AddSlideNoInTransmission(const std::string& filemd5_id, int slide_no){
    {
        std::unique_lock<std::mutex> lock(on_transmission_mutex);
        // add the slide no. to it
        on_transmission_slides[filemd5_id].insert(slide_no);
    }
    return 0;
}

bool SyncDriveServer::RemoveSlideNoInTransmission(const std::string& filemd5_id, int slide_no){
    {
        std::unique_lock<std::mutex> lock(on_transmission_mutex);
        // add the slide no. to it
        on_transmission_slides[filemd5_id].erase(slide_no);
    }
    return true;
}

int SyncDriveServer::GetNextSlideNoInTransmission(const std::string& filemd5_id){
    {
        std::unique_lock<std::mutex> lock(on_transmission_mutex);
        // find the biggest slide_no
        int max_slide_no = -1;
        for(int s: on_transmission_slides[filemd5_id]){
            max_slide_no = std::max(max_slide_no, s);
        }
        TransmissionService transmission_service;
        // try to find the next slide
        auto transmission = transmission_service.GetTransmission(filemd5_id, max_slide_no+1);
        if(transmission){  // if there is
            return max_slide_no + 1;
        }
        else{
            return -1;
        }
    }
}

int SyncDriveServer::AddNextSlideNoInTransmission(const std::string& filemd5_id){
    {
        std::unique_lock<std::mutex> lock(on_transmission_mutex);
        // find the biggest slide_no
        int max_slide_no = -1;
        for(int s: on_transmission_slides[filemd5_id]){
            max_slide_no = std::max(max_slide_no, s);
        }
        TransmissionService transmission_service;
        // try to find the next slide
        auto transmission = transmission_service.GetTransmission(filemd5_id, max_slide_no+1);
        if(transmission){  // if there is
            // add the slide no. to it
            on_transmission_slides[filemd5_id].insert(max_slide_no + 1);
            return max_slide_no + 1;
        }
        else{
            return -1;
        }
    }
}

bool SyncDriveServer::CheckLogin(SyncDriveConnection* connection){
    return client_pool->HasClient(connection->GetFd());
}

std::string SyncDriveServer::GetProtobufString(const std::string& data){
    std::string protobuf(data.c_str()+sizeof(TransmissionHeader), data.length()-sizeof(TransmissionHeader));
    return protobuf;
}

core::v1::optional<bsoncxx::v_noabi::document::value> Dao::GetFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::string& target_field_name){
    try
    {
        return collection.find_one(GenerateFilter(field_names, field_values).view(), GenerateFindOptions(target_field_name));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return {};
}
