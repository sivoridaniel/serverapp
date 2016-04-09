#ifndef GENERICDAO_H_
#define GENERICDAO_H_

#include "../exception/EntityNotFoundException.h"
#include "../exception/InvalidEntityException.h"
#include <exception>
#include "../model/Entity.h"
#include "rocksdb/db.h"

class GenericDao{
protected:
	rocksdb::DB* db;

public:
	GenericDao(rocksdb::DB* db){
		this->db = db;
	}
	virtual ~GenericDao(){this->db = NULL;}
	virtual Entity* get(std::string id) throw(EntityNotFoundException)=0;
	virtual void put(Entity* entity) throw(InvalidEntityException)=0;
};

#endif /* GENERICDAO_H_ */
