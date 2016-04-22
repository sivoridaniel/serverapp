/*
 * AbmUserService.cpp
 *
 *  Created on: 13 de abr. de 2016
 *      Author: pablo
 */

#include "AbmUserService.h"

AbmUserService::AbmUserService() {
	this->userDao = new UserDao();
}

AbmUserService::~AbmUserService() {
	delete userDao;
}

string AbmUserService::createNewUser(string name,string password,string email){
	UserProfile* user = new UserProfile(name,password);
	user->setName(name);
	user->setPassword(password);
	user->setEmail(email);
	this->userDao->put(user);
    delete user;
    /* Read user from database */
    UserProfile* us = (UserProfile*)userDao->get(name);
    string result = us->getName();
    delete us;
    return result;
}
