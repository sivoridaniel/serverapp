/*
 * UserDao.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "UserDao.h"

Entity* UserDao::get(string id) const throw(EntityNotFoundException){

	string json;

	json = DbHelper::get(id, USER);

	UserProfile* usr = new UserProfile(json);

	return usr;
}

void UserDao::put(string id, Entity* e) const throw(InvalidEntityException){

	UserProfile* user = dynamic_cast<UserProfile*>(e);
	if (user==0){
		throw InvalidEntityException();
	}
	std::string json = user->toJson();

	DbHelper::put(id,json,USER);

}
