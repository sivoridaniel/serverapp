/*
 * AbmUserService.cpp
 *
 *  Created on: 13 de abr. de 2016
 *      Author: pablo
 */

#include "AbmUserService.h"

AbmUserService::AbmUserService() {
	this->userDao = new UserDao();
	this->matchDao = new MatchDao();
	this->remoteSharedService = new RemoteSharedService();
}

AbmUserService::~AbmUserService() {
	delete userDao;
	delete matchDao;
	delete remoteSharedService;
}

string AbmUserService::createNewUser(UserProfile* userProfile){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	Match* match = new Match();
	bool existeUsuario = true;

	this->userDao->put(userProfile->getId(),userProfile);
	this->matchDao->put(userProfile->getId(),match);
	delete match;
    /* Read user from database */

    UserProfile* us = (UserProfile*)remoteSharedService->createUser(userProfile);
    string result = us->getName();
    delete us;
    return result;
}

void AbmUserService::modifyUser(UserProfile* userProfile)throw (InvalidEntityException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	try{
		this->userDao->put(userProfile->getId(), userProfile);
	}catch(InvalidEntityException& m){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
				        <<userProfile->getId()<<" no se pudo actualizar."));
		throw m;
	}
}
