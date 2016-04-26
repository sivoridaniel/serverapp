/*
 * UserDao.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */

#include "IUserDao.h"
#include <string>

#ifndef USERDAO_H_
#define USERDAO_H_

class UserDao: public IUserDao{

public:
	UserDao(){};
	void merge(Entity* e) const throw(InvalidEntityException);
	Entity* get(std::string id) const throw(EntityNotFoundException);
	void put(Entity* entity) const throw(InvalidEntityException);
	virtual ~UserDao(){};
};


#endif /* SRC_DAO_USERDAO_H_ */
