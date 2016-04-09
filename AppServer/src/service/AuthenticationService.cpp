/*
 * AuthenticationService.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#include "AuthenticationService.h"

AuthenticationService::AuthenticationService(UserDao* userDao) {
	this->userDao = userDao;
}

AuthenticationService::~AuthenticationService() {
	// TODO Auto-generated destructor stub
}

std::string AuthenticationService::createNewUser(std::string name,
												 std::string password,
												 std::string email){
	UserProfile* user = new UserProfile(name,password);
	user->setName(name);
	user->setPassword(password);
	user->setEmail(email);
	this->userDao->put(user);
    delete user;
    /* Read user from database */
    UserProfile* us = (UserProfile*)userDao->get(name);
    std::string result = us->getName();
    delete us;
    return result;
}
