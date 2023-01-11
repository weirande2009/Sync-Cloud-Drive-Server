#include "dao/dao.h"

Dao::Dao(){
    collection = Database::GetInstance()->GetCollection(Database::CollectionType::User);
}






