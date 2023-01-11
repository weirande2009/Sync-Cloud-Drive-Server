#ifndef TRANSMISSIONSERVICE_H
#define TRANSMISSIONSERVICE_H

#include <optional>

#include "dao/transmission_dao.h"
#include "model/transmission.h"

class TransmissionService{
private:
    TransmissionDao transmission_dao;

public:

    std::optional<std::vector<std::unique_ptr<Transmission>>> GetAllTransmission(const std::string& filemd5_id);
    std::optional<std::unique_ptr<Transmission>> GetTransmission(const std::string& filemd5_id, int slide_no);
    bool HasTransmission(const std::string& filemd5_id);
    bool Add(const std::string& filemd5_id, int slide_no);
    bool AddAll(const std::string& filemd5_id, int start_no, int end_no);
    bool Remove(const std::string& id);
    bool Remove(const std::string& filemd5_id, int slide_no);
};



#endif