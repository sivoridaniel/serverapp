/*
 * AuthenticationService.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#include "AuthenticationService.h"

AuthenticationService::AuthenticationService() {
	this->userDao = new UserDao();
	this->remoteSharedService = new RemoteSharedService();
}

AuthenticationService::AuthenticationService(UserDao* userDao,IRemote* remoteSharedService){
	this->userDao = userDao;
	this->remoteSharedService = new RemoteSharedService();
}

UserProfile* AuthenticationService::getUserLogin(string email, string password)throw(IncorrectPasswordException, EntityNotFoundException){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationService"));
	UserProfile* userProfile = NULL;

	try{

		userProfile=(UserProfile*)userDao->get(email);
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSON: "<<userProfile->toJson()));
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Password base "<<userProfile->getPassword()));
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Password ingresado "<<password));

		if(password.compare("")!=0 && (password.compare(userProfile->getPassword())!=0)){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Usuario de id "<<userProfile->getId()<<" con contraseña incorrecta."));
			delete userProfile;
			throw IncorrectPasswordException();
		}
		string id = userProfile->getId();
		delete userProfile; //Se verifico que sea un usuario registrado, se procede a consultarlo del shared.

		userProfile = remoteSharedService->getUser(id); //se consulta al servicio del shared.
		userProfile->setPassword(password);

	}catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario con email "<<email<<" no se encuentra registrado."));
		delete userProfile;
		throw e;
	}
    return userProfile;
}

AuthenticationService::~AuthenticationService() {
	delete userDao;
	delete remoteSharedService;
}

