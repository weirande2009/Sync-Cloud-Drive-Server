#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "dao/user_dao.h"
#include "model/user.h"

class UserService{
private:
    UserDao user_dao;

public:
    UserService();

    bool Login(const std::string& name, const std::string& password);
    bool Register(const std::string& name, const std::string& password);
    bool ChangePassword(const std::string& id, const std::string& password);
    bool UpdateSyncDirectoryId(const std::string& id, const std::string& sync_directory_id);

};

    



#endif