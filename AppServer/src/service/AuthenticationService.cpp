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

UserProfile* AuthenticationService::getUserLogin(string username, string password)throw(IncorrectPasswordException, EntityNotFoundException){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationService"));
	UserProfile* userProfile = NULL;

	try{

		userProfile=(UserProfile*)userDao->get(username);

		if(!password.compare(userProfile->getPassword())){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Usuario "<<username<<" con contraseña incorrecta."));
			throw IncorrectPasswordException();
		}

	}catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<username<<" no se encuentra registrado."));
		throw e;
	}
    return userProfile;
}

AuthenticationService::~AuthenticationService() {
	delete userDao;
}

