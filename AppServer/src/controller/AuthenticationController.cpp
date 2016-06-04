/*
 * AuthenticationController.cpp
 *
 *  Created on: 16 de abr. de 2016
 *      Author: pablo
 */

#include "AuthenticationController.h"


AuthenticationController::AuthenticationController() {
	this->authenticationService = new AuthenticationService();
	this->abmUserService = new AbmUserService();

}

string AuthenticationController::connect(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("CONNECT AUTHENTICATION CONTROLLER"));

	if (mg_vcmp(&hm->uri, "/login_user") == 0) {
		return event_handler_login_user(nc, hm);
	}else if(mg_vcmp(&hm->uri, "/valid_session") == 0){
		return event_handler_valid_session(nc, hm);
	}
	return STATUS_NOT_FOUND; //Por default devuelve un JSON vacío.

}

string AuthenticationController::event_handler_login_user(struct mg_connection *nc, struct http_message *hm){
	string code = "";
	string token = "";
	string ret_json = "";
	string messageCode = "";
	UserProfile* userProfileBuscado = NULL;
	UserProfile* userProfileConsultado = NULL;
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));
	string password = "";

	try{
		/* Get Form variables */
		string json = string(hm->body.p,hm->body.len);
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << json));
		userProfileBuscado = new UserProfile(json);

		/* Call authentication service */
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("INTENTO LOGUEO"));
		userProfileConsultado=this->authenticationService
		                          ->getUserLogin(userProfileBuscado->getEmail(),userProfileBuscado->getPassword());

		if(userProfileConsultado != NULL){ //Si se pudo loguear, genera el token y modifica el usuario para guardarlo
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("USUARIO LOGUEADO DE ID: "+userProfileConsultado->getId()));
			LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("EMAIL DEL USUARIO A LOGUEAR: "<<userProfileConsultado->getEmail()));
			token = JwToken::generarToken(userProfileConsultado->getEmail());
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN: "+token));
			userProfileConsultado->setToken(token);
			abmUserService->updateToken(userProfileConsultado);
			ret_json = this->createLoginResponse(userProfileConsultado);
			code = STATUS_OK;
		}else{
			code = STATUS_NOK;
			ret_json = this->getGenericJson("false", "No se encontró el usuario");
		}

		delete userProfileBuscado;
		delete userProfileConsultado;

	}catch(EntityNotFoundException& e){
		if (userProfileBuscado != NULL){
			delete userProfileBuscado;
		}
		if (userProfileConsultado !=NULL ){
			delete userProfileConsultado;
		}
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		ret_json = this->getGenericJson("false",e.what());
		code = STATUS_NOT_FOUND;
	}catch(exception & e){
		if (userProfileBuscado != NULL){
			delete userProfileBuscado;
		}
		if (userProfileConsultado !=NULL ){
			delete userProfileConsultado;
		}
		if(&e!=NULL){
			ret_json = this->getGenericJson("false",e.what());
		}else{
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("ERROR INESPERADO"));
			ret_json = this->getGenericJson("false","Error inesperado");
		}
		code = STATUS_NOK;
	}

	this->sendResponse(nc, code, ret_json, token);

	return code;
}

string AuthenticationController::event_handler_valid_session(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));
	string code = valid_session(nc,hm);
    string token = (code.compare(STATUS_NOK)!=0)?code.c_str():""; //Si el resultado es distinto de NOK es porque devolvió el token

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Si es valida la sesion devuelve el token. Resultado: "+token));

	this->sendResponse(nc, code,"", token);

	return code;
}

AuthenticationController::~AuthenticationController() {
	delete this->authenticationService;
	delete this->abmUserService;

}

string AuthenticationController::createLoginResponse(UserProfile* user){
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;
	int i=0;

	root["user"]["name"] = user->getName();
	root["user"]["alias"] = user->getAlias();
	root["user"]["photo"] = user->getPhotoProfile();
	root["user"]["sex"] = user->getSex();
	root["user"]["age"] = user->getAge();
	Location* location = user->getLocation();
	root["user"]["location"]["latitude"] = location->getLatitude();
	root["user"]["location"]["longitude"] = location->getLongitude();
	list<Interest*> interests = user->getInterests();
	if(interests.empty()){
		root["user"]["interests"] = Json::Value(Json::arrayValue);
	}

	for (list< Interest* >::iterator it=interests.begin(); it!=interests.end(); ++it){
		Interest* interest = *it;
		root["user"]["interests"][i]["category"] = interest->getCategory();;
		root["user"]["interests"][i]["value"] = interest->getValue();
		i++;
	}

	root["user"]["email"] = user->getEmail();
	root["user"]["id" ] = user->getId();
	string json = writer.write(root);
	return json;
}
