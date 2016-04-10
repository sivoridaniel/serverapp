/*
 * UserDao.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "UserDao.h"


Entity* UserDao::get(std::string id) throw(EntityNotFoundException){

	std::string json;

	json = DbHelper::get(id, USER);

	UserProfile* usr = new UserProfile(json);

	return usr;
}

void UserDao::put(Entity* e) throw(InvalidEntityException){

	UserProfile* user = dynamic_cast<UserProfile*>(e);
	if (user==0){
		throw InvalidEntityException();
	}
	std::string id = user->getName();
	std::string json = user->toJson();

	DbHelper::put(id,json,USER);

}
