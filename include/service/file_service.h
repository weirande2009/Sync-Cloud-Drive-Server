#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <optional>

#include "dao/file_dao.h"
#include "service/filemd5_service.h"

class FileService{
private:
    FileDao file_dao;
    FileMD5Service filemd5_service;

public:
    FileService();

    std::optional<std::vector<std::unique_ptr<File>>> GetAllByDirectoryId(const std::string& directory_id);
    std::optional<std::vector<std::unique_ptr<File>>> GetAllFinishedByDirectoryId(const std::string& directory_id);
    std::optional<std::unique_ptr<File>> GetFile(const std::string& name, const std::string& directory_id, const std::string& md5);
    std::optional<std::unique_ptr<File>> GetFile(const std::string& id);
    bool AddFile(const std::string& user_id, const std::string& name, const std::string& directory_id, const std::string& md5);
    bool UpdateFileState(const std::string& id, int state);
    bool UpdateAllFileState(const std::string& filemd5_id, int state);
    bool RemoveFile(const std::string& id);
    bool UpdateFileName(const std::string& id, const std::string& name);
    bool HasFile(const std::string& name, const std::string& directory_id, const std::string& md5);
    bool HasFinishedFile(const std::string& name, const std::string& directory_id, const std::string& md5);

};



#endif