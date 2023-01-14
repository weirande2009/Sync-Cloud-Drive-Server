#include "dao/user_dao.h"

UserDao::UserDao(): Dao(){
    
}

/**
 * Generate a doc object for class User
 * @return a doc object
*/
bsoncxx::document::view UserDao::GenerateViewForUser(const User& user){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << user.name
        << "password" << user.password
        << "sync_directory_id" << bsoncxx::oid(user.sync_directory_id)
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate a doc object for class User
 * @return a doc object
*/
bsoncxx::document::view UserDao::GenerateViewForUser(const std::string& name, const std::string& password){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << name
        << "password" << password
        << "sync_directory_id" << 0
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate a user object from view
 * @return a User object
*/
User GenerateUserFromView(const bsoncxx::document::view& view){
    User user;
    user.id = view["_id"].get_oid().value.to_string();
    user.name = view["name"].get_string().value.to_string();
    user.password = view["password"].get_string().value.to_string();
    user.sync_directory_id = view["sync_directory_id"].get_int32();
    return user;
}

/**
 * Get a User object by name
 * @return an optional of User object
*/
std::optional<std::unique_ptr<User>> UserDao::GetByName(const std::string& name){
    return GetOne("name", name, GenerateUserFromView);
}

/**
 * Get ObjectId of a user in string format
 * @return string of an ObjectId
*/
std::optional<std::string> UserDao::GetId(const std::string& name){
    try
    {
        auto result = GetFieldValue("name", name, "_id");
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
 * Check whether there is a user whose name is name
 * @return true: has, false: hasn't
*/
bool UserDao::HasByName(const std::string& name){
    auto result = GetId(name);
    if(result){
        return true;
    }
    else{
        return false;
    }
}

/**
 * Set the user's password
 * @return true: succeed, false: fail
*/
bool UserDao::UpdatePassword(const std::string& id, const std::string& password){
    return UpdateFieldValue("_id", bsoncxx::oid(id), "password", password);
}

/**
 * Set the user's sync directory id
 * @return true: succeed, false: fail
*/
bool UserDao::UpdateSyncDirectoryId(const std::string& id, const std::string& sync_directory_id){
    return UpdateFieldValue("_id", bsoncxx::oid(id), "sync_directory_id", bsoncxx::oid(sync_directory_id));
}

/**
 * Add a new user
 * @return true: succeed, false: fail
*/
bool UserDao::AddUser(const std::string& name, const std::string& password){
    if(HasByName(name)){
        return false;
    }
    std::vector<std::string> field_names = {"name", "password"};
    FieldValues field_values = {name, password};
    return Add(field_names, field_values);
}




