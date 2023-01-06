#ifndef FILEMD5DAO_H
#define FILEMD5DAO_H

#include <optional>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

#include "dao/dao.h"
#include "model/filemd5.h"
#include "utilities/database.h"

static const std::string DISK_DATA_PATH = "../data/";

class FileMd5Dao: public Dao{
private:
    static const int SLIDE_SIZE = 1024;  // slide size of each slide in disk

    bsoncxx::document::view GenerateViewForFileMD5(const FileMD5& file_md5);
    bsoncxx::document::view GenerateViewForFileMD5(const std::string& md5, int size, int slide_num);
    FileMD5 GenerateFileMD5FromView(const bsoncxx::v_noabi::document::view& view);

    bool CreateFileMD5InDisk(const std::string& md5, int size, int slide_num);

public:
    FileMd5Dao();

    // get
    std::optional<FileMD5> GetByMD5(const std::string& md5);
    bool Has(const std::string& md5);
    std::optional<std::string> GetId(const std::string& md5);
    std::optional<std::string> ReadData(const std::string& md5, int slide_no);

    // set
    bool WriteData(const std::string& md5, int slide_no, const std::string& data);
    bool AddFileMD5(const std::string& md5, int size);
    bool Remove(const std::string& id);
    bool UpdateReferenceNum(const std::string& id, int reference_num);
    bool UpdateState(const std::string& id, int state);
};



#endif