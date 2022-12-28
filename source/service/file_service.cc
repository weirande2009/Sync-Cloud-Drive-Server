#include "service/file_service.h"

FileService::FileService(){

}

/**
 * Get all files by directory id
 * @return a list of files
*/
std::optional<std::vector<File>> FileService::GetAllByDirectoryId(const std::string& directory_id){
    return file_dao.GetAllByDirectoryId(directory_id);
}

/**
 * Add a new file
 * @return true: succeed, false: fail
*/
bool FileService::AddFile(const std::string& user_id, const std::string& name, const std::string& directory_id, const std::string& md5){
    return file_dao.AddFile(user_id, name, directory_id, md5);
}

/**
 * Update the state of a file
 * @return true: succeed, false: fail
*/
bool FileService::UpdateFileState(const std::string& id, int state){
    return file_dao.UpdateFileState(id, state);
}

/**
 * Remove a file
 * @return true: succeed, false: fail
*/
bool FileService::RemoveFile(const std::string& id){
    return file_dao.RemoveFile(id);
}

/**
 * Update the name of a file
 * @return true: succeed, false: fail
*/
bool FileService::UpdateFileName(const std::string& id, const std::string& name){
    return file_dao.UpdateFileName(id, name);
}

