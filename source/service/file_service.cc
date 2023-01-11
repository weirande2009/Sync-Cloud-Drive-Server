#include "service/file_service.h"

FileService::FileService(){

}

/**
 * Get all files by directory id
 * @return a list of files
*/
std::optional<std::vector<std::unique_ptr<File>>> FileService::GetAllByDirectoryId(const std::string& directory_id){
    return file_dao.GetAllByDirectoryId(directory_id);
}

/**
 * Get all finished files by directory id
 * @return a list of files
*/
std::optional<std::vector<std::unique_ptr<File>>> FileService::GetAllFinishedByDirectoryId(const std::string& directory_id){
    return file_dao.GetAllFinishedByDirectoryId(directory_id);
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
 * Update the state of all file
 * @return true: succeed, false: fail
*/
bool FileService::UpdateAllFileState(const std::string& filemd5_id, int state){
    return file_dao.UpdateAllFileState(filemd5_id, state);
}

/**
 * Remove a file
 * @return true: succeed, false: fail
*/
bool FileService::RemoveFile(const std::string& id){
    auto md5 = file_dao.GetMd5ById(id);
    if(md5){
        if(file_dao.RemoveFile(id)){
            // remove from filemd5
            auto filemd5_id = filemd5_service.GetId(md5.value());
            if(filemd5_id){
                return filemd5_service.RemoveOne(filemd5_id.value());
            }
        }
    }
    return false;
}

/**
 * Update the name of a file
 * @return true: succeed, false: fail
*/
bool FileService::UpdateFileName(const std::string& id, const std::string& name){
    return file_dao.UpdateFileName(id, name);
}

/**
 * Check whether there is a file
 * @return true: has, false: hasn't
*/
bool FileService::HasFile(const std::string& name, const std::string& directory_id, const std::string& md5){
    auto file_id = file_dao.GetId(name, directory_id, md5);
    if(file_id){
        return true;
    }
    else{
        return false;
    }
}

/**
 * Check whether there is a finished file
 * @return true: has, false: hasn't
*/
bool FileService::HasFinishedFile(const std::string& name, const std::string& directory_id, const std::string& md5){
    auto file = file_dao.GetFile(name, directory_id, md5);
    if(file){
        if(file.value()->state == 1){
            return true;
        }
    }
    return false;
}

/**
 * Get a file object
 * @return a file object
*/
std::optional<std::unique_ptr<File>> FileService::GetFile(const std::string& name, const std::string& directory_id, const std::string& md5){
    return file_dao.GetFile(name, directory_id, md5);
}

/**
 * Get a file object
 * @return a file object
*/
std::optional<std::unique_ptr<File>> FileService::GetFile(const std::string& id){
    return file_dao.GetFile(id);
}

