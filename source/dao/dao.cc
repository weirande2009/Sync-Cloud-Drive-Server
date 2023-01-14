#include "dao/dao.h"

Dao::Dao(){
    collection = Database::GetInstance().GetCollection(Database::CollectionType::User);
}

/**
 * Set field value in collection
 * @return true: succeed, false: fail
*/
bool Dao::UpdateFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_names, field_values).view(), GenerateUpdate(updated_field_names, updated_field_values).view());
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
bool Dao::UpdateAllFieldValue(const std::vector<std::string>& field_names, const FieldValues& field_values, const std::vector<std::string>& updated_field_names, const FieldValues& updated_field_values){
    try
    {
        auto result = collection.update_one(GenerateFilter(field_names, field_values).view(), GenerateUpdate(updated_field_names, updated_field_values).view());
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

bool Dao::RemoveOne(const std::vector<std::string>& field_names, const FieldValues& field_values){
    try
    {
        auto result = collection.delete_one(GenerateFilter(field_names, field_values).view());
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

bool Dao::RemoveAll(const std::vector<std::string>& field_names, const FieldValues& field_values){
    try
    {
        auto result = collection.delete_many(GenerateFilter(field_names, field_values).view());
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

bool Dao::Add(const std::vector<std::string>& field_names, const FieldValues& field_values){
    try
    {
        auto result = collection.insert_one(GenerateFilter(field_names, field_values).view());
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

bsoncxx::document::value Dao::GenerateFilter(const std::vector<std::string>& field_names, const FieldValues& field_values){
    bsoncxx::builder::basic::document basic_builder{};
    int n = field_names.size();
    for(int i=0; i<n; i++){
        auto v_int = std::get_if<int>(&(field_values[i]));
        auto v_str = std::get_if<std::string>(&(field_values[i]));
        auto v_oid = std::get_if<bsoncxx::oid>(&(field_values[i]));
        auto v_bool = std::get_if<bool>(&(field_values[i]));
        if(v_int != nullptr){
            basic_builder.append(bsoncxx::builder::basic::kvp(field_names[i], *v_int));
        }
        if(v_str != nullptr){
            basic_builder.append(bsoncxx::builder::basic::kvp(field_names[i], *v_str));
        }
        if(v_oid != nullptr){
            basic_builder.append(bsoncxx::builder::basic::kvp(field_names[i], *v_oid));
        }
        if(v_bool != nullptr){
            basic_builder.append(bsoncxx::builder::basic::kvp(field_names[i], *v_bool));
        }
    }
    return basic_builder.extract();
}

bsoncxx::document::value Dao::GenerateUpdate(const std::vector<std::string>& field_names, const FieldValues& field_values){
    auto builder = bsoncxx::builder::stream::document{};
    int n = field_names.size();
    builder << "$set" 
            << bsoncxx::builder::stream::open_document;
    for(int i=0; i<n; i++){
        auto v_int = std::get_if<int>(&(field_values[i]));
        auto v_str = std::get_if<std::string>(&(field_values[i]));
        auto v_oid = std::get_if<bsoncxx::oid>(&(field_values[i]));
        auto v_bool = std::get_if<bool>(&(field_values[i]));
        if(v_int != nullptr){
            builder << field_names[i] << *v_int;
        }
        if(v_str != nullptr){
            builder << field_names[i] << *v_str;
        }
        if(v_oid != nullptr){
            builder << field_names[i] << *v_oid;
        }
        if(v_bool != nullptr){
            builder << field_names[i] << *v_bool;
        }
    }
    builder << bsoncxx::builder::stream::close_document;
    return builder.extract();
}

/**
 * Get a find object of only get field of field_name
*/
mongocxx::options::find Dao::GenerateFindOptions(const std::string& field_name){
    auto find_options = mongocxx::v_noabi::options::find();
    find_options.projection(GenerateFilter(field_name, 1).view());
    return find_options;
}

/**
 * Get a find object of only get field of field_names
*/
mongocxx::options::find Dao::GenerateFindOptions(const std::vector<std::string>& field_names){
    int n = field_names.size();
    FieldValues field_values(n, 1);
    auto find_options = mongocxx::v_noabi::options::find();
    find_options.projection(GenerateFilter(field_names, field_values).view());
    return find_options;
}

/**
 * Remove an object by id
*/
bool Dao::RemoveById(const std::string& id){
    return RemoveOne("_id", bsoncxx::oid(id));
}

/**
 * Set field value in collection user
 * @return true: succeed, false: fail
*/
bool Dao::UpdateFieldValueById(const std::string& id, const std::vector<std::string>& field_names, const FieldValues& field_values){
    return UpdateFieldValue("_id", bsoncxx::oid(id), field_names, field_values);
}
