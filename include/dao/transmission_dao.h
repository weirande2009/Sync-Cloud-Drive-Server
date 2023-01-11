#ifndef TRANSMISSIONDAO_H
#define TRANSMISSIONDAO_H

#include <optional>
#include <string>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

#include "dao/dao.h"
#include "model/transmission.h"
#include "utilities/database.h"
    
class TransmissionDao: public Dao{
private:
    bsoncxx::document::view GenerateViewForTransmission(const Transmission& transmission);
    bsoncxx::document::view GenerateViewForTransmission(const std::string filemd5_id, int slide_no);
    Transmission GenerateTransmissionFromView(const bsoncxx::v_noabi::document::view& view);

public:
    TransmissionDao();

    // get
    std::optional<std::vector<std::unique_ptr<Transmission>>> GetAllTransmission(const std::string& filemd5_id);
    std::optional<std::unique_ptr<Transmission>> GetTransmission(const std::string& filemd5_id, int slide_no);
    std::optional<std::string> GetId(const std::string& filemd5_id, const std::string& slide_no);
    bool HasTransmission(const std::string& filemd5_id);

    // set
    bool AddTransmission(const std::string& filemd5_id, int slide_no);
    bool AddAllTransmission(const std::string& filemd5_id, int start_no, int end_no);
    bool RemoveTransmission(const std::string& id);
    bool RemoveTransmission(const std::string& filemd5_id, int slide_no);
};


#endif