#ifndef USERDAO_H
#define USERDAO_H

#include <optional>
#include <string>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

#include "model/user.h"
#include "dao/dao.h"
#include "utilities/database.h"
    
class UserDao: public Dao{
private:
    bsoncxx::document::view GenerateViewForUser(const User& user);
    bsoncxx::document::view GenerateViewForUser(const std::string& name, const std::string& password);
    User GenerateUserFromView(const bsoncxx::document::view& view);

public:
    UserDao();

    // get
    std::optional<User> GetByName(const std::string& name);
    bool HasByName(const std::string& name);
    std::optional<std::string> GetId(const std::string& name);

    // set
    bool UpdatePassword(const std::string& id, const std::string& password);
    bool UpdateSyncDirectoryId(const std::string& id, const std::string& sync_directory_id);
    bool AddUser(const std::string& name, const std::string& password);

};
#endif