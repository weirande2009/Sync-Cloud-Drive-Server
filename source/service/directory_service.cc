#include "service/directory_service.h"

DirectoryService::DirectoryService(){

}

/**
 * Generate a directory tree recursively
*/
void DirectoryService::GenerateDirectoryTree(std::unique_ptr<DirectoryTree>& node, std::unordered_map<std::string, std::vector<Directory*>>& hashtable){
    for(auto& d: hashtable[node->id]){
        auto son = std::make_unique<DirectoryTree>();
        son->id = d->id;
        son->name = d->name;
        son->files = file_service.GetAllByDirectoryId(d->id).value();
        GenerateDirectoryTree(son, hashtable);
        node->sons.emplace_back(std::move(son));
    }
}

/**
 * Get a directory tree by user_id
 * @return a tree of directory
*/
std::optional<std::unique_ptr<DirectoryTree>> DirectoryService::GetDirectoryTreeByUserId(const std::string& user_id){
    auto directories = directory_dao.GetAllByUserId(user_id);
    if(directories){
        const auto& directories_list = directories.value();
        std::unordered_map<std::string, std::vector<Directory*>> id_directory_map;
        // generate root and map
        auto root = std::make_unique<DirectoryTree>();
        for(const auto& d: directories_list){
            if(d->is_root){
                root->id = d->id;
                root->name = d->name;
                root->files = file_service.GetAllFinishedByDirectoryId(d->id).value();
            }
            else{
                id_directory_map[d->parent_id].emplace_back(d.get());
            }
        }
        // generate recursively
        GenerateDirectoryTree(root, id_directory_map);
        return root;
    }
    else{
        return {};
    }
}

/**
 * Update name of a specific id
 * @return true: succeed, false: fail
*/
bool DirectoryService::UpdateName(const std::string& id, const std::string& name){
    return directory_dao.UpdateName(id, name);
}

/**
 * Add a new directory
 * @return true: succeed, false: fail
*/
bool DirectoryService::AddDirectory(const std::string& name, const std::string& parent_id, const std::string& user_id){
    return directory_dao.AddDirectory(name, parent_id, user_id);
}

/**
 * Remove a directory with id
 * @return true: succeed, false: fail
*/
bool DirectoryService::RemoveDirectory(const std::string& id){
    return directory_dao.RemoveDirectory(id);
}

