#include "utilities/database.h"

Database* Database::GetInstance(){
    if(instance == nullptr){
        instance = new Database();
    }
    return instance;
}

Database::Database(){
    
}

void Database::Start(int client_num){
    collection_names[CollectionType::User] = "user";
    collection_names[CollectionType::Directory] = "directory";
    collection_names[CollectionType::File] = "file";
    collection_names[CollectionType::Filemd5] = "filemd5";
    collection_names[CollectionType::Transmission] = "transmission";
}


mongocxx::collection Database::GetCollection(CollectionType collection_type){
    auto client = client_pool.acquire();
    return (*client)[DATABASE_NAME][collection_names[collection_type]];
}

bool Database::CreateEmptyFile(const std::string file_path, int size){
    std::vector<char> buffer(size, 0);
    std::ofstream ofs(file_path.c_str(), std::ios::binary | std::ios::out);
    if (!ofs.write(&buffer[0], buffer.size()))
    {
        std::cerr << "problem writing to file" << std::endl;
        return false;
    }
    ofs.close();
    return true;
}

bool Database::CreateEmptyFile(const std::string file_path, const std::vector<char>& buffer){
    std::ofstream ofs(file_path.c_str(), std::ios::binary | std::ios::out);
    if (!ofs.write(&buffer[0], buffer.size()))
    {
        std::cerr << "problem writing to file" << std::endl;
        return false;
    }
    ofs.close();
    return true;
}


