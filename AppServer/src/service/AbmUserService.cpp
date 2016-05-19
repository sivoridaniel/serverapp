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

string AbmUserService::createNewUser(string name,string password,string token,string email){
	UserProfile* user = new UserProfile(name,password);
	Match* match = new Match();
	user->setName(name);
	user->setPassword(password);
	user->setEmail(email);
	user->setToken(token);
	this->userDao->put(name,user);
	this->matchDao->put(name,match);
    delete user;
    delete match;
    /* Read user from database */
    UserProfile* us = (UserProfile*)userDao->get(name);
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
