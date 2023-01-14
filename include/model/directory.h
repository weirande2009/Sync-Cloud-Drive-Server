#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>

struct Directory{
    std::string id;
    std::string name;
    std::string parent_id;
    std::string user_id;
    bool is_root;
};
    



#endif