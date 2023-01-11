#ifndef DIRECTORYSERVICE_H
#define DIRECTORYSERVICE_H

#include <vector>
#include <string>
#include <optional>
#include <unordered_map>

#include "service/file_service.h"
#include "dao/directory_dao.h"
#include "model/directory.h"
#include "model/file.h"

struct DirectoryTree{
    std::string id;
    std::string name;
    std::vector<std::unique_ptr<DirectoryTree>> sons;
    std::vector<std::unique_ptr<File>> files;

    DirectoryTree(){}
    ~DirectoryTree(){}
    DirectoryTree(const std::string id, const std::string name): id(id), name(name){}
};

class DirectoryService{
private:
    DirectoryDao directory_dao;
    FileService file_service;

    void GenerateDirectoryTree(std::unique_ptr<DirectoryTree>& directory_tree, std::unordered_map<std::string, std::vector<Directory*>>& hashtable);

public:
    DirectoryService();

    std::optional<std::unique_ptr<DirectoryTree>> GetDirectoryTreeByUserId(const std::string& user_id);
    bool UpdateName(const std::string& id, const std::string& name);
    bool AddDirectory(const std::string& name, const std::string& parent_id, const std::string& user_id);
    bool RemoveDirectory(const std::string& id);

};




#endif