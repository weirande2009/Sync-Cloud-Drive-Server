#ifndef DAO_H
#define DAO_H

#include <optional>
#include <vector>
#include <string>
#include <variant>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

#include "utilities/database.h"

typedef std::vector<std::variant<std::string, int, bsoncxx::oid, bool>> FieldValues;
    
class Dao{
private:

protected:
    mongocxx::collection collection;

    template <class T1, class T2, class T3>
    std::optional<std::vector<T1>> GetAll(const std::string& field_name, const T2& field_value, T1 (T3::*generator_from_view)(const bsoncxx::document::view&));

    template <class T1, class T2>
    std::optional<std::vector<T1>> GetAll(const std::vector<std::string>& field_names, const FieldValues& field_values, T1 (T2::*generator_from_view)(const bsoncxx::document::view&));

    template <class T1, class T2, class T3>
    std::optional<T1> GetOne(const std::string& field_name, const T2& field_value, T1 (T3::*generator_from_view)(const bsoncxx::document::view&));

    template <class T1, class T2>
    std::optional<T1> GetOne(const std::vector<std::string>& field_names, const FieldValues& field_values, T1 (T2::*generator_from_view)(const bsoncxx::document::view&));

    template <class T>
    bool UpdateFieldValue(const std::string& id, const std::string field_name, const T& value);
    
    mongocxx::options::find GetIdOptions();
    mongocxx::options::find GetOptions(const std::string& field_name);

    bool RemoveById(const std::string& id);

    bool Add(const bsoncxx::document::view& document);

public:
    Dao();

};

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
 * Get all objects by some field_names with field_values
*/
template <class T1, class T2>
std::optional<std::vector<T1>> Dao::GetAll(const std::vector<std::string>& field_names, const FieldValues& field_values, T1 (T2::*generator_from_view)(const bsoncxx::document::view&)){
    try
    {
        auto builder = bsoncxx::builder::stream::document{};
        int n = field_names.size();
        for(int i=0; i<n; i++){
            builder << field_names[i] << field_values[i]
        }
        builder << bsoncxx::builder::stream::finalize;
        auto result = collection.find(builder);
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
    std::optional<T1> Dao::GetOne(const std::vector<std::string>& field_names, const FieldValues& field_values, T1 (T2::*generator_from_view)(const bsoncxx::document::view&)){
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

#endif