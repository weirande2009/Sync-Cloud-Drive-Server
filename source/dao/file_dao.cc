#include "dao/file_dao.h"

FileDao::FileDao(): Dao(){
    
}

/**
 * Generate a view object for file
 * @return a view object
*/
bsoncxx::document::view FileDao::GenerateViewForFile(const File& file){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << file.name
        << "directory_id" << bsoncxx::oid(file.directory_id)
        << "md5" << file.md5
        << "user_id" << bsoncxx::oid(file.user_id)
        << "state" << file.state
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate a view object for file
 * @return a view object
*/
bsoncxx::document::view FileDao::GenerateViewForFile(const std::string& user_id, const std::string& name, const std::string& directory_id, const std::string& md5){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << name
        << "directory_id" << bsoncxx::oid(directory_id)
        << "md5" << md5
        << "user_id" << bsoncxx::oid(user_id)
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate file object from view
 * @return a File object
*/
File FileDao::GenerateFileFromView(const bsoncxx::v_noabi::document::view& view){
    File file;
    file.id = view["_id"].get_oid().value.to_string();
    file.name = view["name"].get_string().value.to_string();
    file.directory_id = view["directory_id"].get_oid().value.to_string();
    file.md5 = view["md5"].get_string().value.to_string();
    file.user_id = view["user_id"].get_oid().value.to_string();
    file.state = view["state"].get_int32();
    return file;
}

/**
 * Get a list of files from directory_id
 * @return an optional of File object
*/
std::optional<std::vector<std::unique_ptr<File>>> FileDao::GetAllByDirectoryId(const std::string& directory_id){
    return GetAll("directory_id", bsoncxx::oid(directory_id), FileDao::GenerateFileFromView);
}

/**
 * Get a list of finished files from directory_id
 * @return an optional of File object
*/
std::optional<std::vector<std::unique_ptr<File>>> FileDao::GetAllFinishedByDirectoryId(const std::string& directory_id){
    std::vector<std::string> field_names = {"directory_id", "state"};
    FieldValues field_values = {bsoncxx::oid(directory_id), 1};
    return GetAll(field_names, field_values, FileDao::GenerateFileFromView);
}

/**
 * Get a list of files from user_id
 * @return an optional of File object
*/
std::optional<std::vector<std::unique_ptr<File>>> FileDao::GetAllByUserId(const std::string& user_id){
    return GetAll("user_id", bsoncxx::oid(user_id), FileDao::GenerateFileFromView);
}

/**
 * Get a file object
 * @return a file object
*/
std::optional<std::unique_ptr<File>> FileDao::GetFile(const std::string& name, const std::string& directory_id, const std::string& md5){
    std::vector<std::string> field_names = {"name", "directory_id", "md5"};
    FieldValues field_values = {name, bsoncxx::oid(directory_id), md5};
    return GetOne(field_names, field_values, FileDao::GenerateFileFromView);
}

/**
 * Get a file object
 * @return a file object
*/
std::optional<std::unique_ptr<File>> FileDao::GetFile(const std::string& id){
    return GetOne("_id", bsoncxx::oid(id), GenerateFileFromView);
}

/**
 * Get md5 by id
 * @return md5
*/
std::optional<std::string> FileDao::GetMd5ById(const std::string& id){
    try
    {
        auto result = GetFieldValue("_id", bsoncxx::oid(id), "md5");
        if(result){ // if found
            return result.value().view()["md5"].get_string().value.to_string();
        }
        else{
            return {};
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return {};
}

/**
 * Get id of a file
 * @return id
*/
std::optional<std::string> FileDao::GetId(const std::string& name, const std::string& directory_id, const std::string& md5){
    try
    {
        std::vector<std::string> field_names = {"name", "directory_id", "md5"};
        FieldValues field_values = {name, bsoncxx::oid(directory_id), md5};
        auto result = GetFieldValue(field_names, field_values, "_id");
        if(result){ // if found
            return result.value().view()["_id"].get_oid().value.to_string();
        }
        else{
            return {};
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return {};
}

/**
 * Add a file
 * @return true: succeed, false: fail
*/
bool FileDao::AddFile(const std::string& user_id, const std::string& name, const std::string& directory_id, const std::string& md5){
    std::vector<std::string> field_names = {"user_id", "name", "directory_id", "md5"};
    FieldValues field_values = {bsoncxx::oid(user_id), name, bsoncxx::oid(directory_id), md5};
    return Add(field_names, field_values);
}

/**
 * Update the state of a file
 * @return true: succeed, false: fail
*/
bool FileDao::UpdateFileState(const std::string& id, int state){
    return UpdateFieldValue("_id", bsoncxx::oid(id), "state", state);
}

/**
 * Update the state of all files of filemd5_id
 * @return true: succeed, false: fail
*/
bool FileDao::UpdateAllFileState(const std::string& filemd5_id, int state){
    return UpdateAllFieldValue("filemd5_id", bsoncxx::oid(filemd5_id), "state", state);
}

/**
 * Update the name of a file
 * @return true: succeed, false: fail
*/
bool FileDao::UpdateFileName(const std::string& id, const std::string& name){
    return UpdateFieldValue("_id", bsoncxx::oid(id), "name", name);
}

/**
 * Remove a file with id
 * @return true: succeed, false: fail
*/
bool FileDao::RemoveFile(const std::string& id){
    return RemoveById(id);
}










