#ifndef USER_H
#define USER_H

#include <string>

struct User{
    std::string id;
    std::string name;
    std::string password;
    std::string sync_directory_id;
    User();
    User(const std::string& id, const std::string& name, const std::string& password, std::string sync_directory_id);
};



#endif