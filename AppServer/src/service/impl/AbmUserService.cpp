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
		remoteSharedService->createUser(userProfile);
		id = userProfile->getId();


		this->userDao->put(userProfile->getId(),userProfile);
		this->matchDao->put(userProfile->getId(),match);
		delete match;

	}catch(InvalidEntityException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
						        <<userProfile->getId()<<" no se pudo crear."));
		delete match;
		throw e;
	}catch(EntityExistsException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
								        <<userProfile->getId()<<" no se puede crear porque ya existe."));
		delete match;
		throw e;
	}catch(RemoteException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
					   <<userProfile->getId()<<" no se puede crear porque falló el post."));
		delete match;
		throw e;
	}catch(exception& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
							   <<userProfile->getId()<<" no se puede crear porque fallo la escritura a la db."));
		delete match;
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
