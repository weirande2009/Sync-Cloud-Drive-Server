#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>

struct Directory{
    std::string id;
    std::string name;
    std::string parent_id;
    std::string user_id;
    bool is_root;
    Directory(){}
    Directory(const std::string& id, const std::string& name, const std::string& parent_id, const std::string& user_id, bool is_root): 
        id(id), name(name), parent_id(parent_id), user_id(user_id), is_root(is_root){

        }
};
    



#endif