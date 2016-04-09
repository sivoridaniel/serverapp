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
	User* user = new User(name,password);
	user->setName(name);
	user->setPassword(password);
	user->setEmail(email);
	this->userDao->putUser(user);
    delete user;
    /* Read user from database */
    User* us = userDao->getUser(name);
    std::string result = us->getName();
    delete us;
    return result;
}
