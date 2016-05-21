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
}

AbmUserService::~AbmUserService() {
	delete userDao;
	delete matchDao;
}

string AbmUserService::createNewUser(UserProfile* userProfile){
	Match* match = new Match();
	match->setIdUser(userProfile->getName());
	this->userDao->put(userProfile);
	this->matchDao->put(match);
    delete match;
    /* Read user from database */
    UserProfile* us = (UserProfile*)userDao->get(userProfile->getName());
    string result = us->getName();
    delete us;
    return result;
}

void AbmUserService::modifyUser(UserProfile* userProfile)throw (InvalidEntityException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	try{
		this->userDao->put(userProfile);
	}catch(InvalidEntityException& m){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
				        <<userProfile->getId()<<" no se pudo actualizar."));
		throw m;
	}
}
