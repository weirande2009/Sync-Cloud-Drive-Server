#include "dao/dao.h"

Dao::Dao(){
    collection = Database::GetInstance()->GetCollection(Database::CollectionType::User);
}



/**
 * Get a find object of only get field of _id
*/
mongocxx::options::find Dao::GetIdOptions(){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value options = builder
        << "_id" << 1
        << bsoncxx::builder::stream::finalize;
    auto find_options = mongocxx::v_noabi::options::find();
    find_options.projection(options.view());
    return find_options;
}

/**
 * Get a find object of only get field of field_name
*/
mongocxx::options::find Dao::GetOptions(const std::string& field_name){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value options = builder
        << field_name << 1
        << bsoncxx::builder::stream::finalize;
    auto find_options = mongocxx::v_noabi::options::find();
    find_options.projection(options.view());
    return find_options;
}

bool Dao::Remove(const std::vector<std::string>& field_names, const FieldValues& field_values){
    try
    {
        auto builder = bsoncxx::builder::stream::document{};
        int n = field_names.size();
        for(int i=0; i<n; i++){
            builder << field_names[i] << field_values[i];
        }
        builder << bsoncxx::builder::stream::finalize;
        auto result = collection.delete_one(builder.view());
        if(result){
            if(result.value().deleted_count() > 0){
                return true;
            }
        }
        return false;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}


/**
 * Remove an object by id
*/
bool Dao::RemoveById(const std::string& id){
    return Remove("_id", bsoncxx::oid(id));
}

/**
 * Add an object
*/
bool Dao::Add(const bsoncxx::document::view& document){
    try
    {
        auto result = collection.insert_one(document);
        if(result){
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



