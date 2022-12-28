#ifndef TRANSMISSIONSERVICE_H
#define TRANSMISSIONSERVICE_H

#include <optional>

#include "dao/transmission_dao.h"
#include "model/transmission.h"

class TransmissionService{
private:
    TransmissionDao transmission_dao;

public:

    std::optional<Transmission> GetByFilemd5Id(const std::string& filemd5_id);
    bool Add(const std::string& filemd5_id, int slide_no);
    bool AddAll(const std::string& filemd5_id, int start_no, int end_no);
    bool Remove(const std::string& id);
};



#endif