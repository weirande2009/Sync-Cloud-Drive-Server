#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <string>

struct Transmission{
    std::string id;
    std::string filemd5_id;
    int slide_no;
    Transmission();
    Transmission(const std::string& id, const std::string& filemd5_id, int slide_no);
};



#endif