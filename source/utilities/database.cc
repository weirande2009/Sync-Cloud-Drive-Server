#include "utilities/database.h"

Database* Database::GetInstance(){
    if(instance == nullptr){
        instance = new Database();
    }
    return instance;
}

Database::Database(){
    auto c_main = client_pool.acquire();
    clients.insert(std::pair<ClientType, mongocxx::client&>(ClientType::Main, *c_main));
    auto c_transmission = client_pool.acquire();
    clients.insert(std::pair<ClientType, mongocxx::client&>(ClientType::Transmission, *c_main));
    for(auto& p: clients){
        dbs[p.first] = p.second[DATABASE_NAME];
        collections[p.first][CollectionType::User] = dbs[p.first]["user"];
        collections[p.first][CollectionType::Directory] = dbs[p.first]["directory"];
        collections[p.first][CollectionType::File] = dbs[p.first]["file"];
        collections[p.first][CollectionType::Filemd5] = dbs[p.first]["filemd5"];
        collections[p.first][CollectionType::Transmission] = dbs[p.first]["transmission"];
    }
}

mongocxx::collection Database::GetCollection(ClientType client_type, CollectionType collection_type){
    return collections[client_type][collection_type];
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


