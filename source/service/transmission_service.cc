#include "service/transmission_service.h"


TransmissionService::TransmissionService(){

}

/**
 * Get a filemd5 by id
 * @return a file md5 object
*/
std::optional<Transmission> TransmissionService::GetByFilemd5Id(const std::string& filemd5_id){
    return transmission_dao.GetByFilemd5Id(filemd5_id);
}

/**
 * Add a new file md5
 * @return true:succeed, false: fail
*/
bool TransmissionService::Add(const std::string& filemd5_id, int slide_no){
    return transmission_dao.AddTransmission(filemd5_id, slide_no);
}

/**
 * Add all filemd5 from start_no to end_no
 * @return true:succeed, false: fail
*/
bool TransmissionService::AddAll(const std::string& filemd5_id, int start_no, int end_no){
    return transmission_dao.AddAll(filemd5_id, start_no, end_no);
}

/**
 * Remove a file md5 object
 * @return true:succeed, false: fail
*/
bool TransmissionService::Remove(const std::string& id){
    return transmission_dao.Remove(id);
}

