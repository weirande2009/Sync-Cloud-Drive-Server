#include "service/user_service.h"

UserService::UserService(){

}

/**
 * Login
 * @return true:succeed, false: fail
*/
bool UserService::Login(const std::string& name, const std::string& password){
    // find the user object of the name
    auto user = user_dao.GetByName(name);
    if(user){
        auto u = user.value();
        // check whether the password matches
        if(u.password == password){
            return true;
        }
    }
    return false;
}

/**
 * Register
 * @return true:succeed, false: fail
*/
bool UserService::Register(const std::string& name, const std::string& password){
    // check whether the name exists
    if(!user_dao.HasByName(name)){
        // register
        if(user_dao.AddUser(name, password)){
            return true;
        }
    }
    return false;
}

/**
 * Change password
 * @return true:succeed, false: fail
*/
bool UserService::ChangePassword(const std::string& id, const std::string& password){
    return user_dao.UpdatePassword(id, password);
}

/**
 * Update sync directory id
 * @return true:succeed, false: fail
*/
bool UserService::UpdateSyncDirectoryId(const std::string& id, const std::string& sync_directory_id){
    return user_dao.UpdateSyncDirectoryId(id, sync_directory_id);
}

/**
 * Get the id of the name
 * @return id
*/
std::optional<std::string> UserService::GetUserId(const std::string& name){
    return user_dao.GetId(name);
}
