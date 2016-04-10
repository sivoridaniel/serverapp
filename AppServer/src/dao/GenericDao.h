#ifndef GENERICDAO_H_
#define GENERICDAO_H_

#include "../exception/EntityNotFoundException.h"
#include "../exception/InvalidEntityException.h"
#include <exception>
#include "../model/Entity.h"
#include "../utils/DbHelper.h"
#include "../utils/ColumnFamilies.h"

class GenericDao{

public:
	GenericDao(){}
	virtual ~GenericDao(){}
	virtual Entity* get(std::string id) throw(EntityNotFoundException)=0;
	virtual void put(Entity* entity) throw(InvalidEntityException)=0;
};

#endif /* GENERICDAO_H_ */
