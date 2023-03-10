#ifndef FILEDAO_H
#define FILEDAO_H

#include <vector>
#include <string>
#include <optional>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

#include "dao/dao.h"
#include "model/file.h"
#include "utilities/database.h"

class FileDao: public Dao{
private:
    bsoncxx::document::view GenerateViewForFile(const File& file);
    bsoncxx::document::view GenerateViewForFile(const std::string& user_id, const std::string& name, const std::string& directory_id, const std::string& md5);

public:
    FileDao();

    // get
    std::optional<std::vector<std::unique_ptr<File>>> GetAllByDirectoryId(const std::string& directory_id);
    std::optional<std::vector<std::unique_ptr<File>>> GetAllFinishedByDirectoryId(const std::string& directory_id);
    std::optional<std::vector<std::unique_ptr<File>>> GetAllByUserId(const std::string& user_id);
    std::optional<std::unique_ptr<File>> GetFile(const std::string& name, const std::string& directory_id, const std::string& md5);
    std::optional<std::unique_ptr<File>> GetFile(const std::string& id);
    std::optional<std::string> GetMd5ById(const std::string& id);
    std::optional<std::string> GetId(const std::string& name, const std::string& directory_id, const std::string& md5);

    // set
    bool AddFile(const std::string& user_id, const std::string& name, const std::string& directory_id, const std::string& md5);
    bool UpdateFileState(const std::string& id, int state);
    bool UpdateAllFileState(const std::string& filemd5_id, int state);
    bool UpdateFileName(const std::string& id, const std::string& name);
    bool RemoveFile(const std::string& id);

};

File GenerateFileFromView(const bsoncxx::v_noabi::document::view& view);



#endif