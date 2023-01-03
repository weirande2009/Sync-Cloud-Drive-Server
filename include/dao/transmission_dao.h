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
    std::optional<Transmission> GetByFilemd5Id(const std::string& filemd5_id);
    std::optional<std::string> GetId(const std::string& filemd5_id, const std::string& slide_no);

    // set
    bool AddTransmission(const std::string& filemd5_id, int slide_no);
    bool AddAll(const std::string& filemd5_id, int start_no, int end_no);
    bool Remove(const std::string& id);

};


#endif