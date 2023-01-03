#include "dao/dao.h"

Dao::Dao(){
    collection = Database::GetInstance()->GetCollection(Database::CollectionType::User);
}

/**
 * Set field value in collection user
 * @return true: succeed, false: fail
*/
template <class T>
bool Dao::UpdateFieldValue(const std::string& id, const std::string field_name, const T& value){
    try
    {
        auto result = collection.update_one(
            bsoncxx::builder::stream::document{} 
            << "_id" << bsoncxx::oid(id)
            << bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{} 
            << "$set" 
            << bsoncxx::builder::stream::open_document 
            << field_name << value
            << bsoncxx::builder::stream::close_document 
            << bsoncxx::builder::stream::finalize);
        if(result){
            if(result.value().modified_count() > 0){
                return true;
            }
            else{
                return false;
            }
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
 * Get all objects by a field_name with field_value
*/
template <class T1, class T2, class T3>
std::optional<std::vector<T1>> Dao::GetAll(const std::string& field_name, const T2& field_value, T1 (T3::*generator_from_view)(const bsoncxx::v_noabi::document::view&)){
    try
    {
        auto result = collection.find(
            bsoncxx::builder::stream::document{}
            << field_name << field_value
            << bsoncxx::builder::stream::finalize
        );
        std::vector<T1> objects;
        for(auto& doc: result){
            objects.push_back(generator_from_view(doc));
        }
        return objects;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return {};
}

/**
 * Get one by field_name with field_value
*/
template <class T1, class T2, class T3>
std::optional<T1> Dao::GetOne(const std::string& field_name, const T2& field_value, T1 (T3::*generator_from_view)(const bsoncxx::v_noabi::document::view&)){
    try
    {
        bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(
            bsoncxx::builder::stream::document{}
            << field_name << field_value
            << bsoncxx::builder::stream::finalize
        );
        if(result){ // if found
            return generator_from_view(result.value().view());
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

template <class T1, class T2>
    std::optional<T1> Dao::GetOne(const std::vector<std::string>& field_name, const FieldValues& field_values, T1 (T2::*generator_from_view)(const bsoncxx::document::view&)){
    try
    {
        auto builder = bsoncxx::builder::stream::document{};
        int n = field_names.size();
        for(int i=0; i<n; i++){
            builder << field_names[i] << field_values[i]
        }
        builder << bsoncxx::builder::stream::finalize;
        bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(builder);
        if(result){ // if found
            return generator_from_view(result.value().view());
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



