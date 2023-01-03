#include "dao/transmission_dao.h"

TransmissionDao::TransmissionDao(): Dao(){
    
}

/**
 * Generate a view object for transmission
 * @return a view object
*/
bsoncxx::document::view TransmissionDao::GenerateViewForTransmission(const Transmission& transmission){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "filemd5_id" << bsoncxx::oid(transmission.filemd5_id)
        << "slide_no" << transmission.slide_no
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate a view object for file_md5
 * @return a view object
*/
bsoncxx::document::view TransmissionDao::GenerateViewForTransmission(const std::string filemd5_id, int slide_no){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "filemd5_id" << bsoncxx::oid(filemd5_id)
        << "slide_no" << slide_no
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate file_md5 object from view
 * @return a FileMD5 object
*/
Transmission TransmissionDao::GenerateTransmissionFromView(const bsoncxx::v_noabi::document::view& view){
    Transmission transmission;
    transmission.id = view["_id"].get_oid().value.to_string();
    transmission.filemd5_id = view["filemd5_id"].get_oid().value.to_string();
    transmission.slide_no = view["slide_no"].get_int32();
    return transmission;
}

/**
 * Get a transmission object by filemd5_id
*/
std::optional<Transmission> TransmissionDao::GetByFilemd5Id(const std::string& filemd5_id){
    return GetOne("filemd5_id", filemd5_id, GenerateTransmissionFromView);
}

/**
 * Get id of a transmission object
 * @return the id of the transmission object
*/
std::optional<std::string> TransmissionDao::GetId(const std::string& filemd5_id, const std::string& slide_no){
    try
    {
        auto find_options = GetIdOptions();
        auto result = collection.find_one(
            bsoncxx::builder::stream::document{}
            << "filemd5_id" << filemd5_id
            << "slide_no" << slide_no
            << bsoncxx::builder::stream::finalize
        );
        if(result){ // if found
            return result.value().view()["_id"].get_oid().value.to_string();
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
 * Add a transmission object
 * @return true:succeed, false: fail
*/
bool TransmissionDao::AddTransmission(const std::string& filemd5_id, int slide_no){
    return Add(GenerateViewForTransmission(filemd5_id, slide_no));
}

/**
 * Add all from start_no to end_no
 * @return true:succeed, false: fail
*/
bool TransmissionDao::AddAll(const std::string& filemd5_id, int start_no, int end_no){
    for(int i=start_no; i<=end_no; i++){
        if(!Add(GenerateViewForTransmission(filemd5_id, i))){
            return false;
        }
    }
}

/**
 * Remove a transmission object by id
 * @return true:succeed, false: fail
*/
bool TransmissionDao::Remove(const std::string& id){
    return RemoveById(id);
}







