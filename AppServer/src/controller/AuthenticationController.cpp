/*
 * AuthenticationController.cpp
 *
 *  Created on: 16 de abr. de 2016
 *      Author: pablo
 */

#include "AuthenticationController.h"


AuthenticationController::AuthenticationController() {
	this->authenticationService = new AuthenticationService();
	this->abmService = new AbmUserService();
	this->jwToken = new JwToken();
}

string AuthenticationController::login(string username, string password){
	string msg_response = NULL;
	UserProfile* userProfile = NULL;
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));

	try{
		userProfile = this->authenticationService->getUserLogin(username,password);

		if(userProfile != NULL){ //Si se pudo loguear, genera el token y modifica el usuario para guardarlo
			string token = jwToken->generarToken(username);
			userProfile->setToken(token);
			abmService->modifyUser(userProfile);
			msg_response = userProfile->toJson();
		}

	}catch(exception & e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		msg_response = "{ \"result\": \"failed\" }";
	}



	return msg_response;
}

AuthenticationController::~AuthenticationController() {
	delete this->authenticationService;
	delete this->abmService;
	delete this->jwToken;
}
