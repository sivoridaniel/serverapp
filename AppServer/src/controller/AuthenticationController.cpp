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
	this->jwToken = new JwToken();
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
	return ""; //Por default devuelve un JSON vacío.

}

string AuthenticationController::event_handler_login_user(struct mg_connection *nc, struct http_message *hm){
	string msg_response = "";
	string token="";
	UserProfile* userProfile = NULL;
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));

	char vecIdUser[20], vecPassword[20];

	try{

	/* Get Form variables */

	mg_get_http_var(&hm->body, "name", vecIdUser, sizeof(vecIdUser));
	mg_get_http_var(&hm->body, "password", vecPassword, sizeof(vecPassword));

	/* Send headers */
	mg_printf(nc, "%s","HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");


	/* Call authentication service */
	string username(vecIdUser);
	string password(vecPassword);


		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("INTENTO LOGUEO"));
		userProfile = this->authenticationService->getUserLogin(username,password);


		if(userProfile != NULL){ //Si se pudo loguear, genera el token y modifica el usuario para guardarlo
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("USUARIO LOGUEADO: "+userProfile->getName()));
			token = jwToken->generarToken(username);
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN: "+token));
			userProfile->setToken(token);
			abmUserService->modifyUser(userProfile);
			msg_response = "status ok";

		}else{
			msg_response = "status nok";
		}



	}catch(exception & e){
		if(&e!=NULL){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		}else{
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("ERROR INESPERADO"));
		}
		msg_response = "status failed";
	}

	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" , \"token\": \"%s\"}", msg_response.c_str(),token.c_str());
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */

	return msg_response;
}

string AuthenticationController::event_handler_valid_session(struct mg_connection *nc, struct http_message *hm){
	string msg_response="";
	string token="";
	UserProfile* userProfile = NULL;
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));

	char vecToken[260];

	try{
	/* Get Form variables */
	mg_get_http_var(&hm->body, "token", vecToken, sizeof(vecToken));

	/* Send headers */
	mg_printf(nc, "%s","HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Call authentication service */
	string token(vecToken);

		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN RECIBIDO: "+token));

		bool isTokenValid = jwToken->isTokenValid(token);


		if(isTokenValid){
			LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("TOKEN VALIDO"));
			string username = jwToken->getUserName(token);
			token = jwToken->generarToken(username); //se genera el nuevo token, con el nuevo timestamp, renovando la sesion
			userProfile=authenticationService->getUserLogin(username,""); //se pasa el password en vacío
			userProfile->setToken(token);
			abmUserService->modifyUser(userProfile); //se modifica el usuario para asignarle el nuevo token
			msg_response = "status ok";
		}else{
			msg_response = "status nok";
		}



	}catch(exception & e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		msg_response = "failed";
	}

	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" , \"token\": \"%s\"}", msg_response.c_str(),token.c_str());
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */

	return msg_response;
}

AuthenticationController::~AuthenticationController() {
	delete this->authenticationService;
	delete this->abmUserService;
	delete this->jwToken;
}
