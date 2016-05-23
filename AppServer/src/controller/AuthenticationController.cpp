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
	string msg_response = "";
	string token = "";
	string ret_json = "";
	UserProfile* userProfileBuscado = NULL;
	UserProfile* userProfileConsultado = NULL;
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));

	try{
		/* Get Form variables */
		string json = string(hm->body.p,hm->body.len);
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << json));
		userProfileBuscado = new UserProfile(json);

		/* Call authentication service */
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("INTENTO LOGUEO"));
		userProfileConsultado=this->authenticationService
		                          ->getUserLogin(userProfileBuscado->getName(),userProfileBuscado->getPassword());

		if(userProfileConsultado != NULL){ //Si se pudo loguear, genera el token y modifica el usuario para guardarlo
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("USUARIO LOGUEADO: "+userProfileConsultado->getName()));
			token = JwToken::generarToken(userProfileConsultado->getName());
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN: "+token));
			userProfileConsultado->setToken(token);
			abmUserService->updateToken(userProfileConsultado);
			ret_json = userProfileConsultado->toSharedJson();
			msg_response = STATUS_OK;

		}else{
			msg_response = STATUS_NOK;
			ret_json = "{ \"success\": \"false\", \"data\": \"Bad Request: No se encontró el usuario\"}";
		}

		delete userProfileBuscado;
		delete userProfileConsultado;

	}catch(exception & e){
		if(&e!=NULL){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			ret_json = string("{ \"success\": \"false\", \"data\": \"")+e.what()+string("\"}");
		}else{
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("ERROR INESPERADO"));
			ret_json = "{ \"success\": \"false\", \"data\": \"Error inesperado\"}";
		}
		msg_response = STATUS_NOK;

	}

	int code = atoi(msg_response.c_str());
	string error = (code == atoi(STATUS_NOK.c_str()))?"Bad Request":"OK";

	/* Send headers */
	mg_printf(nc,"HTTP/1.1 %d %s\r\nTransfer-Encoding: chunked\r\n"
			     "Content-Type: application/json; charset=UTF-8\r\n"
			     "Token: %s \r\n\r\n",code,error.c_str(),token.c_str());
	mg_printf_http_chunk(nc,"%s",ret_json.c_str());
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */


	return msg_response;
}

string AuthenticationController::event_handler_valid_session(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AuthenticationController"));
	string result = valid_session(nc,hm);
	string status = (result.compare(STATUS_NOK)!=0)?STATUS_OK:STATUS_NOK;
	int code = atoi(status.c_str());
	string error = (code == atoi(STATUS_NOK.c_str()))?"Bad Request":"OK";
    string token = (result.compare(STATUS_NOK)!=0)?result.c_str():""; //Si el resultado es distinto de NOK es porque devolvió el token

	/* Send headers */
	mg_printf(nc,"HTTP/1.1 %d %s\r\nTransfer-Encoding: chunked\r\n"
			     "Content-Type: application/json; charset=UTF-8\r\n"
			     "Token: %s \r\n\r\n",code,error.c_str(),token.c_str());

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Si es valida la sesion devuelve el token. Resultado: "+token));

	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */

	return status;
}

AuthenticationController::~AuthenticationController() {
	delete this->authenticationService;
	delete this->abmUserService;

}
