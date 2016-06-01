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
	/**
	 * Se obtiene el usuario con email pasado por parámetro.
	 * @param email
	 * @throw EntityNotFoundException
	 * @return Entity*
	 */
	Entity* get(string email) const throw(EntityNotFoundException);
	/**
	 * Se inserta el usuario con el email que le corresponde.
	 *
	 * @param email
	 * @param entity
	 * @throw InvalidEntityException
	 */
	void put(string email, Entity* entity) const throw(InvalidEntityException);
	virtual ~UserDao(){};
};


#endif /* SRC_DAO_USERDAO_H_ */
