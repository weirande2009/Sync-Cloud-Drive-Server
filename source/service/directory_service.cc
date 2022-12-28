#include "service/directory_service.h"

DirectoryService::DirectoryService(){

}

/**
 * Generate a directory tree recursively
*/
void DirectoryService::GenerateDirectoryTree(DirectoryTree& node, std::unordered_map<std::string, std::vector<Directory>&>& hashtable){
    for(auto& d: hashtable[node.id]){
        DirectoryTree directory_tree(d.id, d.name);
        directory_tree.files = file_service.GetAllByDirectoryId(d.id).value();
        GenerateDirectoryTree(directory_tree, hashtable);
        node.sons.push_back(directory_tree);
    }
}

/**
 * Get a directory tree by user_id
 * @return a tree of directory
*/
std::optional<DirectoryTree> DirectoryService::GetDirectoryTreeByUserId(const std::string& user_id){
    auto directories = directory_dao.GetAllByUserId(user_id);
    if(directories){
        auto& directories_list = directories.value();
        std::unordered_map<std::string, std::vector<Directory>&> id_directory_map;
        // generate root and map
        DirectoryTree root;
        for(auto& d: directories_list){
            if(d.is_root){
                root.id = d.id;
                root.name = d.name;
                root.files = file_service.GetAllByDirectoryId(d.id).value();
            }
            else{
                id_directory_map[d.parent_id].push_back(d);
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

