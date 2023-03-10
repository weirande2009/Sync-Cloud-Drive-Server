#ifndef DIRECTORYDAO_H
#define DIRECTORYDAO_H

#include <vector>
#include <optional>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

#include "model/directory.h"
#include "dao/dao.h"
#include "utilities/database.h"

class DirectoryDao: public Dao{
private:
    bsoncxx::document::view GenerateViewForDirectory(const Directory& directory);
    bsoncxx::document::view GenerateViewForDirectory(const std::string& name, const std::string& parent_id, const std::string& user_id);
    

public:
    DirectoryDao();

    // get
    std::optional<std::vector<std::unique_ptr<Directory>>> GetAllByUserId(const std::string& user_id);
    std::optional<std::vector<std::unique_ptr<Directory>>> GetAllByParentId(const std::string& parent_id);
    std::optional<std::string> GetDirectoryId(const std::string& name, const std::string& parent_id, const std::string& user_id);
    bool Has(const std::string& name, const std::string& parent_id, const std::string& user_id);
    bool Has(const std::string& id);
    std::optional<std::unique_ptr<Directory>> GetRootDirectory(const std::string& user_id);

    // set
    bool UpdateName(const std::string& id, const std::string& name);
    bool AddDirectory(const std::string& name, const std::string& parent_id, const std::string& user_id);
    bool RemoveDirectory(const std::string& id);
};

Directory GenerateDirectoryFromView(const bsoncxx::document::view& view);



#endif