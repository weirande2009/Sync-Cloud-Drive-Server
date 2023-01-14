#ifndef FILE_H
#define FILE_H

#include <string>

struct File{
    std::string id;
    std::string name;
    std::string directory_id;
    std::string md5;
    std::string user_id;
    int state;
};



#endif