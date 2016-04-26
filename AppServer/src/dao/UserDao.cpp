/*
 * UserDao.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "UserDao.h"

Entity* UserDao::get(std::string id) const throw(EntityNotFoundException){

	std::string json;

	json = DbHelper::get(id, USER);

	UserProfile* usr = new UserProfile(json);

	return usr;
}

void UserDao::merge(Entity* e) const throw(InvalidEntityException){

	UserProfile* user = dynamic_cast<UserProfile*>(e);

	if (user==0){
			throw InvalidEntityException();
	}

	std::string id = user->getName();
	std::string json = user->toJson();

	DbHelper::merge(id,json,USER);

}

void UserDao::put(Entity* e) const throw(InvalidEntityException){

	UserProfile* user = dynamic_cast<UserProfile*>(e);
	if (user==0){
		throw InvalidEntityException();
	}
	std::string id = user->getName();
	std::string json = user->toJson();

	DbHelper::put(id,json,USER);

}
