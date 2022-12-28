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
    Database::ClientType client_type;
    mongocxx::collection collection;

    template <class T1, class T2, class T3>
    std::optional<std::vector<T1>> GetAll(const std::string& field_name, const T2& field_value, T1 (T3::*generator_from_view)(const bsoncxx::document::view&));

    template <class T1, class T2, class T3>
    std::optional<T1> GetOne(const std::string& field_name, const T2& field_value, T1 (T3::*generator_from_view)(const bsoncxx::document::view&));

    template <class T1, class T2>
    std::optional<T1> GetOne(const std::vector<std::string>& field_name, const FieldValues& field_values, T1 (T2::*generator_from_view)(const bsoncxx::document::view&));

    template <class T>
    bool UpdateFieldValue(const std::string& id, const std::string field_name, const T& value);
    
    mongocxx::options::find GetIdOptions();
    mongocxx::options::find GetOptions(const std::string& field_name);

    bool RemoveById(const std::string& id);

    bool Add(const bsoncxx::document::view& document);

public:
    Dao();
    Dao(Database::ClientType client_type);

};


#endif