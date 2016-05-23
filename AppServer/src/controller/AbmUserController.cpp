/*
 * UserProfileController.cpp
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "AbmUserController.h"

AbmUserController::AbmUserController() {
	abmService = new AbmUserService();
}

string AbmUserController::connect(struct mg_connection *nc, struct http_message *hm){

	if(mg_vcmp(&hm->uri, "/user/newuser") == 0){
		return event_handler_new_user(nc,hm);
	}else if(mg_vcmp(&hm->uri, "/user/getuser") == 0){
		return event_handler_get_user(nc,hm);
	}
	return "404"; //Por default devuelve NOT_FOUND.
}

string AbmUserController::event_handler_new_user(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("UserLoginController"));

	string json = string(hm->body.p,hm->body.len);

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << json));

	UserProfile* userProfile = new UserProfile(json);

	/* Send headers */
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Guardando en db usuario " << userProfile->getName()));

	string result = abmService->createNewUser(userProfile);

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }","200");
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */

	return result;
}

string AbmUserController::event_handler_get_user(struct mg_connection *nc, struct http_message *hm){
	return "{}";
}

AbmUserController::~AbmUserController() {
	delete abmService;
}
