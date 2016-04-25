/*
 * UserDao.h
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#ifndef IUSERDAO_H_
#define IUSERDAO_H_
#include "../model/UserProfile.h"
#include "GenericDao.h"

class IUserDao: public GenericDao{

public:
	virtual ~IUserDao(){};
	virtual void merge(Entity* e) throw(InvalidEntityException)=0;

};

#endif /* USERDAO_H_ */
