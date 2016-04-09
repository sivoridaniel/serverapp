/*
 * UserDao.h
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#ifndef USERDAO_H_
#define USERDAO_H_
#include "../model/UserProfile.h"
#include "GenericDao.h"

class UserDao : public GenericDao{

public:
	UserDao(){};
	virtual ~UserDao(){};
	Entity* get(std::string id) throw(EntityNotFoundException);
	void put(Entity* user) throw(InvalidEntityException);

};

#endif /* USERDAO_H_ */
