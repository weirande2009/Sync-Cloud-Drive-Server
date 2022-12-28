#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <optional>

#include "dao/file_dao.h"

class FileService{
private:
    FileDao file_dao;

public:
    FileService();

    std::optional<std::vector<File>> GetAllByDirectoryId(const std::string& directory_id);
    bool AddFile(const std::string& user_id, const std::string& name, const std::string& directory_id, const std::string& md5);
    bool UpdateFileState(const std::string& id, int state);
    bool RemoveFile(const std::string& id);
    bool UpdateFileName(const std::string& id, const std::string& name);

};



#endif