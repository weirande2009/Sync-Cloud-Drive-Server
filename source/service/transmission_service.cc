#include "service/transmission_service.h"


TransmissionService::TransmissionService(){

}

/**
 * Get all transmissions by filemd5_id
 * @return a list of transmission objects
*/
std::optional<std::vector<Transmission>> TransmissionService::GetAllTransmission(const std::string& filemd5_id){
    return transmission_dao.GetAllTransmission(filemd5_id);
}

/**
 * Get a transmission by filemd5_id and slide no.
 * @return a transmission object
*/
std::optional<Transmission> TransmissionService::GetTransmission(const std::string& filemd5_id, int slide_no){
    return transmission_dao.GetTransmission(filemd5_id, slide_no);
}

/**
 * Add a new transmission
 * @return true:succeed, false: fail
*/
bool TransmissionService::Add(const std::string& filemd5_id, int slide_no){
    return transmission_dao.AddTransmission(filemd5_id, slide_no);
}

/**
 * Add all transmission from start_no to end_no
 * @return true:succeed, false: fail
*/
bool TransmissionService::AddAll(const std::string& filemd5_id, int start_no, int end_no){
    return transmission_dao.AddAllTransmission(filemd5_id, start_no, end_no);
}

/**
 * Remove a transmission object
 * @return true:succeed, false: fail
*/
bool TransmissionService::Remove(const std::string& id){
    return transmission_dao.RemoveTransmission(id);
}

/**
 * Remove a transmission object
 * @return true:succeed, false: fail
*/
bool TransmissionService::Remove(const std::string& filemd5_id, int slide_no){
    return transmission_dao.RemoveTransmission(filemd5_id, slide_no);
}

/**
 * Check whether there is a transmission by filemd5_id
 * @return true:has, false: hasn't
*/
bool TransmissionService::HasTransmission(const std::string& filemd5_id){
    return transmission_dao.HasTransmission(filemd5_id);
}

