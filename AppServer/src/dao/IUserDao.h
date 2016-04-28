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
/**
 * Clase abstracta para poder mockear UserDao con gmock.
 */
class IUserDao: public GenericDao{

public:
	virtual ~IUserDao(){};

};

#endif /* USERDAO_H_ */
