#include "dao/filemd5_dao.h"

FileMd5Dao::FileMd5Dao(): Dao(){
    
}

/**
 * Create a filemd5 directory with slides in disk
*/
bool FileMd5Dao::CreateFileMD5InDisk(const std::string& md5, int size, int slide_num){
    // md5 is the directory name of the file
    // find whether there is still a filemd5 in disk
    struct stat sb;
    std::string folder_path = DISK_DATA_PATH + md5;
    if (stat(folder_path.c_str(), &sb) != 0){  // already exists
        return false;
    }
    // create directory
    if (mkdir(folder_path.c_str(), 0777) == -1){
        return false;
    }
    // create slide file
    std::vector<char> buf(SLIDE_SIZE, 0);
    for(int i=0; i<slide_num-1; i++){
        std::string file_path = folder_path + "/" + std::to_string(i);
        if(!Database::GetInstance()->CreateEmptyFile(file_path, buf)){
            return false;
        }
    }
    // create the file for the last slide
    std::string file_path = folder_path + "/" + std::to_string(slide_num-1);
    int last_slide_size = size - (slide_num-1)*SLIDE_SIZE;
    if(!Database::GetInstance()->CreateEmptyFile(file_path, last_slide_size)){
        return false;
    }
    return true;
}


/**
 * Generate a view object for file_md5
 * @return a view object
*/
bsoncxx::document::view FileMd5Dao::GenerateViewForFileMD5(const FileMD5& file_md5){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "md5" << file_md5.md5
        << "size" << file_md5.size
        << "slide_num" << file_md5.slide_num
        << "reference_num" << file_md5.reference_num
        << "state" << file_md5.state
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate a view object for file_md5
 * @return a view object
*/
bsoncxx::document::view FileMd5Dao::GenerateViewForFileMD5(const std::string& md5, int size, int slide_num){
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "md5" << md5
        << "size" << size
        << "slide_num" << slide_num
        << "reference_num" << 1
        << "state" << 0
        << bsoncxx::builder::stream::finalize;
    return doc_value.view();
}

/**
 * Generate file_md5 object from view
 * @return a FileMD5 object
*/
FileMD5 FileMd5Dao::GenerateFileMD5FromView(const bsoncxx::v_noabi::document::view& view){
    FileMD5 file_md5;
    file_md5.id = view["_id"].get_oid().value.to_string();
    file_md5.md5 = view["md5"].get_string().value.to_string();
    file_md5.size = view["size"].get_int32();
    file_md5.slide_num = view["slide_num"].get_int32();
    file_md5.reference_num = view["reference_num"].get_int32();
    file_md5.state = view["state"].get_int32();
    return file_md5;
}

/**
 * Get a file_md5 object by md5
 * @return a FileMD5 object
*/
std::optional<FileMD5> FileMd5Dao::GetByMD5(const std::string& md5){
    return GetOne("md5", md5, GenerateFileMD5FromView);
}

/**
 * Check whether there is a file_md5 object with md5
 * @return true: has, false: hasn't
*/
bool FileMd5Dao::Has(const std::string& md5){
    auto result = GetId(md5);
    if(result){
        return true;
    }
    else{
        return false;
    }
}

/**
 * Get the id of
*/
std::optional<std::string> FileMd5Dao::GetId(const std::string& md5){
    try
    {
        auto find_options = GetIdOptions();
        auto result = collection.find_one(
            bsoncxx::builder::stream::document{}
            << "md5" << md5
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
 * Read the data of a specific md5 with slide_no
 * @return a string of data
*/
std::optional<std::string> FileMd5Dao::ReadData(const std::string& md5, int slide_no){
    // check whether the slide file exists
    struct stat sb;
    std::string folder_path = DISK_DATA_PATH + md5 + "/" + std::to_string(slide_no);
    if (stat(folder_path.c_str(), &sb) != 0){  // already exists
        return {};
    }
    // open file
    std::ifstream myfile(folder_path);
    if(!myfile.is_open()){
        return {};
    }
    // read data
    std::string data;
    myfile >> data;
    myfile.close();
    if(data.size() > SLIDE_SIZE){
        return {};
    }
    return data;
}

/**
 * Write the data of a specific md5 with slide_no
 * @return true: succeed, false: fail
*/
bool FileMd5Dao::WriteData(const std::string& md5, int slide_no, const std::string& data){
    // check the data size
    if(data.size() > SLIDE_SIZE){
        return false;
    }
    // check whether the slide file exists
    struct stat sb;
    std::string folder_path = DISK_DATA_PATH + md5 + "/" + std::to_string(slide_no);
    if (stat(folder_path.c_str(), &sb) != 0){  // already exists
        return false;
    }
    // open file
    std::ofstream myfile(folder_path);
    if(!myfile.is_open()){
        return {};
    }
    // write data
    myfile << data;
    myfile.close();
    return true;
}

/**
 * Add a file_md5 object
 * @return true: succeed, false: fail
*/
bool FileMd5Dao::AddFileMD5(const std::string& md5, int size){
    int slide_num = size % SLIDE_SIZE == 0 ? size / SLIDE_SIZE : size / SLIDE_SIZE + 1;
    if(Add(GenerateViewForFileMD5(md5, size, slide_num))){
        CreateFileMD5InDisk(md5, size, slide_num);
        return true;
    }
    else{
        return false;
    }
}

/**
 * Remove a file_md5 object by id
 * @return true: succeed, false: fail
*/
bool FileMd5Dao::Remove(const std::string& id){
    return RemoveById(id);
}

/**
 * Update reference number
 * @return true: succeed, false: fail
*/
bool FileMd5Dao::UpdateReferenceNum(const std::string& id, int reference_num){
    return UpdateFieldValue(id, "reference_num", reference_num);
}

/**
 * Update state
 * @return true: succeed, false: fail
*/
bool FileMd5Dao::UpdateState(const std::string& id, int state){
    return UpdateFieldValue(id, "state", state);
}
