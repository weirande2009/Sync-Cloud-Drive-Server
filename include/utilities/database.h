#ifndef DATABASE_H
#define DATABASE_H

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

#include <string>
#include <fstream>
#include <vector>

#include "utilities/database_common.h"

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

private:
    mongocxx::instance database_instance{}; // This should be done only once.
    mongocxx::pool client_pool{
        mongocxx::uri{
          DATABASE_CONNECTION_URI
        }
    };
    std::unordered_map<CollectionType, std::string> collection_names;
    
    Database();

public:
    Database(Database const&) = delete;
    void operator=(Database const&) = delete;
    static Database& GetInstance();
    void Start(int client_num);
    mongocxx::collection GetCollection(CollectionType collection_type);
    bool CreateEmptyFile(const std::string file_path, int size);
    bool CreateEmptyFile(const std::string file_path, const std::vector<char>& buffer);

};



#endif