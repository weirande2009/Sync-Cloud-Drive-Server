#ifndef DATABASE_H
#define DATABASE_H

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

static const char* DATABASE_CONNECTION_URI = "mongodb://admin:123456@127.0.0.1:27017/syncdrive";
static const char* DATABASE_NAME = "syncdrive";

class Database{
public:
    enum CollectionType{
        User = 0,
        Directory = 1,
        File = 2,
        Filemd5 = 3,
        Transmission = 4,
    };

    enum ClientType{
        Main = 0,
        Transmission = 1,
    };

private:
    static Database* instance;
    mongocxx::instance database_instance{}; // This should be done only once.
    mongocxx::pool client_pool{
        mongocxx::uri{
          DATABASE_CONNECTION_URI
        }
    };
    std::unordered_map<ClientType, mongocxx::client&> clients;
    std::unordered_map<ClientType, mongocxx::database> dbs;
    std::unordered_map<ClientType, std::unordered_map<CollectionType, mongocxx::collection>> collections;

    Database();

public:

    static Database* GetInstance();
    mongocxx::collection GetCollection(ClientType client_type, CollectionType collection_type);
    bool CreateEmptyFile(const std::string file_path, int size);
    bool CreateEmptyFile(const std::string file_path, const std::vector<char>& buffer);

};



#endif