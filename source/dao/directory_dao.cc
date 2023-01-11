#include "dao/directory_dao.h"

DirectoryDao::DirectoryDao(): Dao(){
    
}

/**
 * Generate a view object for directory
 * @return a view object
*/
bsoncxx::document::view DirectoryDao::GenerateViewForDirectory(const Directory& directory){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << directory.name
        << "parent_id" << bsoncxx::oid(directory.parent_id)
        << "user_id" << bsoncxx::oid(directory.user_id)
        << "is_root" << false
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate a view object for directory
 * @return a view object
*/
bsoncxx::document::view DirectoryDao::GenerateViewForDirectory(const std::string& name, const std::string& parent_id, const std::string& user_id){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << name
        << "parent_id" << bsoncxx::oid(parent_id)
        << "user_id" << bsoncxx::oid(user_id)
        << "is_root" << false
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate directory object from view
 * @return a Directory object
*/
Directory DirectoryDao::GenerateDirectoryFromView(const bsoncxx::document::view& view){
    Directory directory;
    directory.id = view["_id"].get_oid().value.to_string();
    directory.name = view["name"].get_string().value.to_string();
    directory.parent_id = view["parent_id"].get_oid().value.to_string();
    directory.user_id = view["user_id"].get_oid().value.to_string();
    directory.is_root = view["is_root"].get_bool();
    return directory;
}

/**
 * Get a list of directory from user_id
 * @return an optional of Directory object
*/
std::optional<std::vector<std::unique_ptr<Directory>>> DirectoryDao::GetAllByUserId(const std::string& user_id){
    return GetAll<Directory, bsoncxx::oid, DirectoryDao>("_id", bsoncxx::oid(user_id), DirectoryDao::GenerateDirectoryFromView);
}

/**
 * Get a list of directory from parent_id
 * @return an optional of Directory object
*/
std::optional<std::vector<std::unique_ptr<Directory>>> DirectoryDao::GetAllByParentId(const std::string& parent_id){
    return GetAll<Directory, bsoncxx::oid, DirectoryDao>("parent_id", bsoncxx::oid(parent_id), DirectoryDao::GenerateDirectoryFromView);
}

/**
 * Get the id of a directory
 * @return an optional of User object
*/
std::optional<std::string> DirectoryDao::GetDirectoryId(const std::string& name, const std::string& parent_id, const std::string& user_id){
    try
    {
        std::vector<std::string> field_names = {"name", "parent_id", "user_id"};
        FieldValues field_values = {name, bsoncxx::oid(parent_id), bsoncxx::oid(user_id)};
        std::string target_field_name = "_id";
        auto result = GetFieldValue(field_names, field_values, target_field_name);
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
 * Check whether there is a already a directory
 * @return true: has, false: hasn't
*/
bool DirectoryDao::Has(const std::string& name, const std::string& parent_id, const std::string& user_id){
    auto result = GetDirectoryId(name, parent_id, user_id);
    if(result){
        return true;
    }
    else{
        return false;
    }
}

/**
 * Check whether there is a already a directory
 * @return true: has, false: hasn't
*/
bool DirectoryDao::Has(const std::string& id){
    try
    {
        auto result = GetFieldValue("_id", bsoncxx::oid(id), "_id");
        if(result){ // if found
            return true;
        }
        else{
            return false;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}

/**
 * Get the root directory of a user
 * @return the root directory
*/
std::optional<std::unique_ptr<Directory>> DirectoryDao::GetRootDirectory(const std::string& user_id){
    std::vector<std::string> field_names = {"user_id", "is_root"};
    FieldValues field_values = {bsoncxx::oid(user_id), true};
    return GetOne(field_names, field_values, GenerateDirectoryFromView);
}

/**
 * Update the name of a directory
 * @return true: succeed, false: fail
*/
bool DirectoryDao::UpdateName(const std::string& id, const std::string& name){
    return UpdateFieldValue("_id", bsoncxx::oid(id), "name", name);
}

/**
 * Add a new directory
*/
bool DirectoryDao::AddDirectory(const std::string& name, const std::string& parent_id, const std::string& user_id){
    if(Has(name, parent_id, user_id)){
        return false;
    }
    std::vector<std::string> field_names = {"name", "parent_id", "user_id"};
    FieldValues field_values = {name, bsoncxx::oid(parent_id), bsoncxx::oid(user_id)};
    return Add(field_names, field_values);
}

/**
 * Delete a directory with id
 * @return true: succeed, false: fail
*/
bool DirectoryDao::RemoveDirectory(const std::string& id){
    return RemoveById(id);
}




