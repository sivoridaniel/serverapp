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

AuthenticationService::AuthenticationService(UserDao* userDao){
	this->userDao = userDao;
}

UserProfile* AuthenticationService::getUserLogin(string username, string password)throw(IncorrectPasswordException, EntityNotFoundException){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationService"));
	UserProfile* userProfile = NULL;

	try{

		userProfile=(UserProfile*)userDao->get(username);
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSON: "<<userProfile->toJson()));
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Password base "<<userProfile->getPassword()));
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Password ingresado "<<password));

		if(password.compare("")!=0 && (password.compare(userProfile->getPassword())!=0)){
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

