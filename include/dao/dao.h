#pragma once

#include <optional>
#include <vector>
#include <string>
#include <variant>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>

#include "utilities/database.h"

typedef std::vector<std::variant<std::string, int, bsoncxx::oid, bool>> FieldValues;
    
class Dao{
private:

protected:

    mongocxx::collection collection;

    template <typename T1, typename T2>
    std::optional<std::vector<std::unique_ptr<T1>>> GetAll(const std::string& field_name, const T2& field_value, T1 (*generator_from_view)(const bsoncxx::document::view&));

    template <typename T>
    std::optional<std::vector<std::unique_ptr<T>>> GetAll(const std::vector<std::string>& field_names, const FieldValues& field_values, T (*generator_from_view)(const bsoncxx::document::view&));

    template <typename T1, typename T2>
    std::optional<std::unique_ptr<T1>> GetOne(const std::string& field_name, const T2& field_value, T1 (*generator_from_view)(const bsoncxx::document::view&));

    template <typename T>
    std::optional<std::unique_ptr<T>> GetOne(const std::vector<std::string>& field_names, const FieldValues& field_values, T (*generator_from_view)(const bsoncxx::document::view&));

    core::v1::optional<bsoncxx::v_noabi::document::value> GetFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::string& target_field_name);

    template <typename T>
    core::v1::optional<bsoncxx::v_noabi::document::value> GetFieldValue(const std::string& field_name, const T& field_value, const std::string& target_field_name);

    template <typename T>
    bool RemoveOne(const std::string& field_name, const T& field_value);

    bool RemoveOne(const std::vector<std::string>& field_names, const FieldValues& field_values);

    template <typename T>
    bool RemoveAll(const std::string& field_name, const T& field_value);

    bool RemoveAll(const std::vector<std::string>& field_names, const FieldValues& field_values);

    template <typename T1, typename T2>
    bool UpdateFieldValue(const std::string& field_name, const T1& field_value, const std::string& updated_field_name, const T2& updated_field_value);

    template <typename T>
    bool UpdateFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::string& updated_field_name, const T& updated_field_value);

    template <typename T1, typename T2>
    bool UpdateAllFieldValue(const std::string& field_name, const T1& field_value, const std::string& updated_field_name, const T2& updated_field_value);

    template <typename T>
    bool UpdateAllFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::string& updated_field_name, const T& updated_field_value);

    template <typename T>
    bool UpdateFieldValue(const std::string& field_name, const T& field_value, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values);

    bool UpdateFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values);

    template <typename T>
    bool UpdateAllFieldValue(const std::string& field_name, const T& field_value, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values);

    bool UpdateAllFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values);

    bool Add(const std::vector<std::string>& field_names, const FieldValues& field_values);

    // document generator
    template <class T>
    bsoncxx::document::value GenerateFilter(const std::string& field_name, const T& field_value);

    bsoncxx::document::value GenerateFilter(const std::vector<std::string>& field_names, const FieldValues& field_values);

    template <class T>
    bsoncxx::document::value GenerateUpdate(const std::string& field_name, const T& field_value);

    bsoncxx::document::value GenerateUpdate(const std::vector<std::string>& field_names, const FieldValues& field_values);

    mongocxx::options::find GenerateFindOptions(const std::string& field_name);

    mongocxx::options::find GenerateFindOptions(const std::vector<std::string>& field_names);

    // id related
    bool RemoveById(const std::string& id);

    template <typename T>
    bool UpdateFieldValueById(const std::string& id, const std::string& field_name, const T& value);

    bool UpdateFieldValueById(const std::string& id, const std::vector<std::string>& field_names, const FieldValues& field_values);

    template <typename T1, typename T2>
    std::optional<std::unique_ptr<T1>> GetById(const std::string& id, T1 (T2::*generator_from_view)(const bsoncxx::document::view&));

public:
    Dao();

};

/**
 * Set field value in collection
 * @return true: succeed, false: fail
*/
template <typename T1, typename T2>
bool Dao::UpdateFieldValue(const std::string& field_name, const T1& field_value, const std::string& updated_field_name, const T2& updated_field_value){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_name, field_value).view(), GenerateUpdate(updated_field_name, updated_field_value).view());
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
 * Set field value in collection
 * @return true: succeed, false: fail
*/
template <typename T>
bool Dao::UpdateFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::string& updated_field_name, const T& updated_field_value){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_names, field_values).view(), GenerateUpdate(updated_field_name, updated_field_value).view());
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
 * Set field value in collection for all
 * @return true: succeed, false: fail
*/
template <typename T1, typename T2>
bool Dao::UpdateAllFieldValue(const std::string& field_name, const T1& field_value, const std::string& updated_field_name, const T2& updated_field_value){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_name, field_value).view(), GenerateUpdate(updated_field_name, updated_field_value).view());
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
 * Set field value in collection for all
 * @return true: succeed, false: fail
*/
template <typename T>
bool Dao::UpdateAllFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::string& updated_field_name, const T& updated_field_value){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_names, field_values).view(), GenerateUpdate(updated_field_name, updated_field_value).view());
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
 * Set field value in collection
 * @return true: succeed, false: fail
*/
template <typename T>
bool Dao::UpdateFieldValue(const std::string& field_name, const T& field_value, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_name, field_value).view(), GenerateUpdate(updated_field_names, updated_field_values).view());
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
 * Set field value in collection for all
 * @return true: succeed, false: fail
*/
template <typename T>
bool Dao::UpdateAllFieldValue(const std::string& field_name, const T& field_value, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_name, field_value).view(), GenerateUpdate(updated_field_names, updated_field_values).view());
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

template <typename T>
core::v1::optional<bsoncxx::v_noabi::document::value> Dao::GetFieldValue(const std::string& field_name, const T& field_value, const std::string& target_field_name){
    try
    {
        return collection.find_one(GenerateFilter(field_name, field_value).view(), GenerateFindOptions(target_field_name));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return {};
}

/**
 * Get all objects by a field_name with field_value
*/
template <typename T1, typename T2>
std::optional<std::vector<std::unique_ptr<T1>>> Dao::GetAll(const std::string& field_name, const T2& field_value, T1 (*generator_from_view)(const bsoncxx::v_noabi::document::view&)){
    try
    {
        auto result = collection.find(GenerateFilter(field_name, field_value).view());
        std::vector<std::unique_ptr<T1>> objects;
        for(auto& doc: result){
            objects.emplace_back(std::move(std::make_unique<T1>(generator_from_view(doc))));
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
template <typename T>
std::optional<std::vector<std::unique_ptr<T>>> Dao::GetAll(const std::vector<std::string>& field_names, const FieldValues& field_values, T (*generator_from_view)(const bsoncxx::document::view&)){
    try
    {
        auto result = collection.find(GenerateFilter(field_names, field_values).view());
        std::vector<std::unique_ptr<T>> objects;
        for(auto& doc: result){
            objects.emplace_back(std::move(std::make_unique<T>(generator_from_view(doc))));
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
template <typename T1, typename T2>
std::optional<std::unique_ptr<T1>> Dao::GetOne(const std::string& field_name, const T2& field_value, T1 (*generator_from_view)(const bsoncxx::v_noabi::document::view&)){
    try
    {
        auto result = collection.find_one(GenerateFilter(field_name, field_value).view());
        if(result){ // if found
            return std::make_unique<T1>(generator_from_view(result.value().view()));
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

template <typename T>
std::optional<std::unique_ptr<T>> Dao::GetOne(const std::vector<std::string>& field_names, const FieldValues& field_values, T (*generator_from_view)(const bsoncxx::document::view&)){
    try
    {
        auto result = collection.find_one(GenerateFilter(field_names, field_values).view());
        if(result){ // if found
            return std::make_unique<T>(generator_from_view(result.value().view()));
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

template <typename T>
bool Dao::RemoveOne(const std::string& field_name, const T& field_value){
    try
    {
        auto result = collection.delete_one(GenerateFilter(field_name, field_value).view());
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

template <typename T>
bool Dao::RemoveAll(const std::string& field_name, const T& field_value){
    try
    {
        auto result = collection.delete_many(GenerateFilter(field_name, field_value).view());
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

template <typename T>
bsoncxx::document::value Dao::GenerateFilter(const std::string& field_name, const T& field_value){
    bsoncxx::builder::basic::document basic_builder{};
    basic_builder.append(bsoncxx::builder::basic::kvp(field_name, field_value));
    return basic_builder.extract();
}

template <class T>
bsoncxx::document::value Dao::GenerateUpdate(const std::string& field_name, const T& field_value){
    return bsoncxx::builder::stream::document{} 
        << "$set" 
        << bsoncxx::builder::stream::open_document 
        << field_name << field_value
        << bsoncxx::builder::stream::close_document 
        << bsoncxx::builder::stream::finalize;
}

/**
 * Set field value in collection user
 * @return true: succeed, false: fail
*/
template <typename T>
bool Dao::UpdateFieldValueById(const std::string& id, const std::string& field_name, const T& field_value){
    return UpdateFieldValue("_id", bsoncxx::oid(id), field_name, field_value);
}

template <typename T1, typename T2>
std::optional<std::unique_ptr<T1>> Dao::GetById(const std::string& id, T1 (T2::*generator_from_view)(const bsoncxx::document::view&)){
    return GetOne("_id", bsoncxx::oid(id), generator_from_view);
}
