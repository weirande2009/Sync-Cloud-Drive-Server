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

/**
 * Remove an object by id
*/
bool Dao::RemoveById(const std::string& id){
    try
    {
        auto result = collection.delete_one(
            bsoncxx::builder::stream::document{}
            << "_id" << bsoncxx::oid(id)
            << bsoncxx::builder::stream::finalize);
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



