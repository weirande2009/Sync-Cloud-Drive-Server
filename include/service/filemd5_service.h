#ifndef FILEMD5SERIVCE_H
#define FILEMD5SERIVCE_H

#include <optional>

#include "dao/filemd5_dao.h"
#include "model/filemd5.h"

class FileMD5Service{
private:
    FileMd5Dao filemd5_dao;

public:
    FileMD5Service();

    std::optional<FileMD5> GetByMD5(const std::string& md5);
    bool Has(const std::string& md5);
    std::optional<std::string> GetId(const std::string& md5);
    std::optional<std::string> ReadData(const std::string& md5, int slide_no);
    bool WriteData(const std::string& md5, int slide_no, const std::string& data);
    bool Add(const std::string& md5, int size, int slide_num);
    bool Remove(const std::string& id);
    bool UpdateReferenceNum(const std::string& id, int reference_num);
    bool UpdateState(const std::string& id, int state);


};

    



#endif