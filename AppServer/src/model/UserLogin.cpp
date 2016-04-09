/*
 * UserLogin.cpp
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "UserLogin.h"


UserLogin::UserLogin(int idUser, string username, string password) {
	this->idUser = idUser;
	this->username = username;
	this->password = password;
}

int UserLogin::getIdUser(){
	return this->idUser;
}

string UserLogin::getUserName(){
	return this->username;
}

string UserLogin::getPassword(){
	return this->password;
}


UserLogin::~UserLogin() {
	// TODO Auto-generated destructor stub
}

