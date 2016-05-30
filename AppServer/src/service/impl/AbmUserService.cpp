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

void AbmUserService::existeUsuario(string email)throw (EntityExistsException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));

	try{
		UserProfile* userProfile = (UserProfile*)this->userDao->get(email);
		delete userProfile;
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario con email "<<email<<" ya está registrado."));
		throw EntityExistsException();
	}catch(EntityNotFoundException&e){
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("El usuario con email "<<email<<" no esta registrado. Se continua con la registración."));
	}

}

string AbmUserService::createNewUser(UserProfile* userProfile){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	Match* match = new Match();
	string id="";

	try{
		existeUsuario(userProfile->getEmail());

		remoteSharedService->createUser(userProfile);

		this->userDao->put(userProfile->getEmail(),userProfile);
		this->matchDao->put(userProfile->getId(),match);
		delete match;

	}catch(InvalidEntityException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con email "
						        <<userProfile->getEmail()<<" no se pudo crear."));
		delete match;
		throw e;
	}catch(EntityExistsException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con email "
								        <<userProfile->getEmail()<<" no se puede crear porque ya existe."));
		delete match;
		throw e;
	}catch(RemoteException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con email "
					   <<userProfile->getEmail()<<" no se puede crear porque falló el post."));
		delete match;
		throw e;
	}catch(exception& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con email "
							   <<userProfile->getEmail()<<" no se puede crear porque fallo la escritura a la db."));
		delete match;
		throw e;
	}

    return id; //Devuelve el id del usuario generado por el servicio externo (shared server)
}

void AbmUserService::updateToken(UserProfile* userProfile)throw (InvalidEntityException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	try{
		UserProfile* userProfileAux = (UserProfile*)this->userDao->get(userProfile->getEmail());
		delete userProfileAux; //Si no arroja la excepcion (EntityNotFound) quiere decir que existe
		this->userDao->put(userProfile->getEmail(), userProfile);
	}catch(InvalidEntityException& m){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con email "
				        <<userProfile->getEmail()<<" no pudo actualizar su token."));
		throw m;
	}catch(EntityNotFoundException& n){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con email "
								        <<userProfile->getEmail()<<" no se encuentra."));
		throw n;
	}catch(exception& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error inesperado."));
		throw e;
	}
}

void AbmUserService::modifyUser(UserProfile* userProfile)throw (InvalidEntityException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));
	try{
		UserProfile* userProfileAux = (UserProfile*)this->userDao->get(userProfile->getEmail());
		userProfile->setId(userProfileAux->getId());
		delete userProfileAux; //Si no arroja la excepcion (EntityNotFound) quiere decir que existe
		this->remoteSharedService->updateUser(userProfile);
		//TODO: validar que exista localmente
		this->userDao->put(userProfile->getEmail(), userProfile); //Si cambia el email, nombre de usuario, etc
	}catch(InvalidEntityException& m){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con id "
				        <<userProfile->getId()<<" no se pudo actualizar."));
		throw m;
	}catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<userProfile->getName()<<" con email "
						        <<userProfile->getEmail()<<" no se encuentra."));
		throw e;
	}catch(RemoteException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error en comunicacion con shared service."));
		throw e;
	}catch(exception& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}
}

list<Interest*> AbmUserService::getInterests(){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserService"));

	try{
		list<Interest*> intereses = this->remoteSharedService->getInterests();
		return intereses;
	}catch(RemoteException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}catch(JsonParseException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}catch(exception& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}

}
