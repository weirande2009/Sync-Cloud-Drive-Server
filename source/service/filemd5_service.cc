#include "service/filemd5_service.h"

FileMD5Service::FileMD5Service(){

}

/**
 * Get a file md5 object by id
 * @return a file_md5 object
*/
std::optional<FileMD5> FileMD5Service::GetByMD5(const std::string& md5){
    return filemd5_dao.GetByMD5(md5);
}

/**
 * Check whether there is a file of md5
 * @return true: has, false: hasn't
*/
bool FileMD5Service::Has(const std::string& md5){
    return filemd5_dao.Has(md5);
}

/**
 * Get id of a md5
 * @return id
*/
std::optional<std::string> FileMD5Service::GetId(const std::string& md5){
    return filemd5_dao.GetId(md5);
}

/**
 * Read data of a slide of a file
 * @return data
*/
std::optional<std::string> FileMD5Service::ReadData(const std::string& md5, int slide_no){
    return filemd5_dao.ReadData(md5, slide_no);
}

/**
 * Write data into a slide of a file
 * @return true: succeed, false: fail
*/
bool FileMD5Service::WriteData(const std::string& md5, int slide_no, const std::string& data){
    return filemd5_dao.WriteData(md5, slide_no, data);
}

/**
 * Add a file md5
 * @return true: succeed, false: fail
*/
bool FileMD5Service::Add(const std::string& md5, int size){
    // add a filemd5 object
    if(filemd5_dao.AddFileMD5(md5, size)){
        // get the newly added filemd5's id
        auto filemd5 = filemd5_dao.GetByMD5(md5);
        if(filemd5){
            // add transmission objects
            if(transmission_service.AddAll(filemd5.value().id, 0, filemd5.value().slide_num-1)){
                return true;
            }
        }
    }
    return false;
}

/**
 * Remvoe a file md5
 * @return true: succeed, false: fail
*/
bool FileMD5Service::Remove(const std::string& id){
    return filemd5_dao.Remove(id);
}

/**
 * Update the reference number of a filemd5
 * @return true: succeed, false: fail
*/
bool FileMD5Service::UpdateReferenceNum(const std::string& id, int reference_num){
    return filemd5_dao.UpdateReferenceNum(id, reference_num);
}

/**
 * Update the state of a filemd5
 * @return true: succeed, false: fail
*/
bool FileMD5Service::UpdateState(const std::string& id, int state){
    return filemd5_dao.UpdateState(id, state);
}





