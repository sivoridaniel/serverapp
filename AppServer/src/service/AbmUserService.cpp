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
	string id="";

	try{
		this->userDao->put(userProfile->getId(),userProfile);
		this->matchDao->put(userProfile->getId(),match);
		delete match;
		/* Read user from database */

		remoteSharedService->createUser(userProfile);
		id = userProfile->getId();

	}catch(InvalidEntityException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
						        <<userProfile->getId()<<" no se pudo crear."));
		throw e;
	}catch(EntityExistsException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
								        <<userProfile->getId()<<" no se puede crear porque ya existe."));
		throw e;
	}catch(RemoteException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
					   <<userProfile->getId()<<" no se puede crear porque falló el post."));
		throw e;
	}

    return id; //Devuelve el id del usuario generado por el servicio externo (shared server)
}

void AbmUserService::updateToken(UserProfile* userProfile)throw (InvalidEntityException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	try{
		this->userDao->put(userProfile->getId(), userProfile);
	}catch(InvalidEntityException& m){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
				        <<userProfile->getId()<<" no pudo actualizar su token."));
		throw m;
	}
}

void AbmUserService::modifyUser(UserProfile* userProfile)throw (InvalidEntityException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	try{
		this->userDao->put(userProfile->getId(), userProfile); //Si cambia el email, nombre de usuario, etc
		this->remoteSharedService->updateUser(userProfile);
	}catch(InvalidEntityException& m){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
				        <<userProfile->getId()<<" no se pudo actualizar."));
		throw m;
	}
}
