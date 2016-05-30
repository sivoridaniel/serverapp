/*
 * UserDao.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */

#include "GenericDao.h"
#include "../model/UserProfile.h"
#include <string>

#ifndef USERDAO_H_
#define USERDAO_H_

using namespace std;

class UserDao: public GenericDao{

public:
	UserDao(){};
	Entity* get(string email) const throw(EntityNotFoundException);
	void put(string email, Entity* entity) const throw(InvalidEntityException);
	virtual ~UserDao(){};
};


#endif /* SRC_DAO_USERDAO_H_ */
