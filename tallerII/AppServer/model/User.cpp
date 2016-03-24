/*
 * User.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "User.h"

User::User(std::string name, std::string password) {
	this->name = name;
	this->password = password;
}

User::~User() {
	this->name = "";
	this->password = "";
}

