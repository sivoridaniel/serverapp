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
	}else if(mg_vcmp(&hm->uri, "/user/updateuser") == 0){
		return event_handler_update_user(nc,hm);
	}
	return "404"; //Por default devuelve NOT_FOUND.
}

string AbmUserController::event_handler_new_user(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("UserLoginController"));

	string json = string(hm->body.p,hm->body.len);
	string return_value = "";
	string strcode = "200";

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << json));

	UserProfile* userProfile = new UserProfile(json);

	try{
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Guardando en db usuario " << userProfile->getName()));

		abmService->createNewUser(userProfile);

		return_value = (string("{\"success\":\"false\", \"data\":\"")+userProfile->getId()+string("\"}"));

	}catch(InvalidEntityException& e){
		strcode = "400";
		return_value = "{ \"success\": \"false\", \"data\": \"El usuario no se pudo crear\"}";
	}catch(EntityExistsException& e){
		strcode = "400";
		return_value = "{ \"success\": \"false\", \"data\": \"El usuario ya existe\"}";
	}catch(RemoteException& e){
		strcode = "404";
		return_value = "{ \"success\": \"false\", \"data\": \"Error desconocido\"}";
	}

	int code = atoi(strcode.c_str());
	string error = (code == atoi(STATUS_NOK.c_str()))?"Bad Request":"OK";

	/* Send headers */
	mg_printf(nc,"HTTP/1.1 %d %s\r\nTransfer-Encoding: chunked\r\n"
			"Content-Type: application/json; charset=UTF-8\r\n",code,error.c_str());
	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", return_value.c_str());
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */

	delete userProfile;

	return strcode;
}

string AbmUserController::event_handler_update_user(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("UserLoginController"));
	string json = string(hm->body.p,hm->body.len);
	string return_value = "";
	string strcode = "200";

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << json));

	UserProfile* userProfile = new UserProfile(json);

	try{
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Actualizando usuario con id " << userProfile->getId()));
		abmService->modifyUser(userProfile);
		return_value = string("{\"success\":\"false\", \"data\":\"profile update\"}");
	}catch(InvalidEntityException& e){
		strcode = "400";
		return_value = (string("{ \"success\": \"false\", \"data\": \"")+e.what()+string("\"}"));
	}catch(EntityNotFoundException& e){
		strcode = "400";
		return_value = (string("{ \"success\": \"false\", \"data\": \"")+e.what()+string("\"}"));
	}catch(RemoteException& e){
		strcode = "404";
		return_value = (string("{ \"success\": \"false\", \"data\": \"")+e.what()+string("\"}"));
	}

	int code = atoi(strcode.c_str());
	string error = (code == atoi(STATUS_NOK.c_str()))?"Bad Request":"OK";

	/* Send headers */
	mg_printf(nc,"HTTP/1.1 %d %s\r\nTransfer-Encoding: chunked\r\n"
			"Content-Type: application/json; charset=UTF-8\r\n",code,error.c_str());
	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", return_value.c_str());
	mg_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */

	delete userProfile;
	return strcode;
}

AbmUserController::~AbmUserController() {
	delete abmService;
}
