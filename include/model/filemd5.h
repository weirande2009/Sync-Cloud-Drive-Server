#ifndef FILEMD5_H
#define FILEMD5_H

#include <string>

struct FileMD5{
    std::string id;
    std::string md5;
    int size;
    int slide_num;
    int reference_num;
    int state;
};


#endif