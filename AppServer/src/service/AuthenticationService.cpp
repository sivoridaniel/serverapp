/*
 * AuthenticationService.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#include "AuthenticationService.h"

AuthenticationService::AuthenticationService() {
	this->userDao = new UserDao();
}

AuthenticationService::~AuthenticationService() {
	delete userDao;
}


