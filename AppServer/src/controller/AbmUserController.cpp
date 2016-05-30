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
		if (mg_vcmp(&hm->method, "POST") == 0) {
			return event_handler_new_user(nc,hm);
		}
	}else if(mg_vcmp(&hm->uri, "/user/updateuser") == 0){
		if (mg_vcmp(&hm->method, "PUT") == 0) {
			return event_handler_update_user(nc,hm);
		}
	}
	return STATUS_NOT_FOUND; //Por default devuelve NOT_FOUND.
}

string AbmUserController::event_handler_new_user(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("UserLoginController"));

	string data = string(hm->body.p,hm->body.len);
	string json = "";
	string code = STATUS_OK;
	string messageCode = "";
	UserProfile* userProfile=NULL;

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << data));

	try{
		userProfile = new UserProfile(data);
		userProfile->setRegistracionUser(true);

		try{
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Guardando en db usuario " << userProfile->getName()));

			abmService->createNewUser(userProfile);
			json = this->getGenericJson("true", userProfile->getId());
			code = STATUS_OK;
			delete userProfile;
		}catch(InvalidEntityException& e){
			code = STATUS_NOK;
			json = this->getGenericJson("true", e.what());
			delete userProfile;
		}catch(EntityExistsException& e){
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(RemoteException& e){
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}
		catch(exception& e){
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}

	}catch(JsonParseException& e){
		code = STATUS_NOK;
		json = this->getGenericJson("false", e.what());
		delete userProfile;
	}

	this->sendResponse(nc,code,json,"");

	return code;
}

string AbmUserController::event_handler_update_user(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("UserLoginController"));
	string data = string(hm->body.p,hm->body.len);
	string json = "";
	string code = STATUS_OK;
	UserProfile* userProfile=NULL;

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << data));

	try{
		userProfile = new UserProfile(data);

		try{
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Actualizando usuario con id " << userProfile->getId()));
			abmService->modifyUser(userProfile);
			json = this->getGenericJson("true", "profile updated");
			code = STATUS_OK;
			delete userProfile;
		}catch(InvalidEntityException& e){
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(EntityNotFoundException& e){
			code = STATUS_NOT_FOUND;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(RemoteException& e){
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(exception& e){
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}


	}catch(JsonParseException& e){
		code = STATUS_NOK;
		json = this->getGenericJson("false", e.what());
		delete userProfile;
	}

	this->sendResponse(nc,code,json, "");

	return code;
}

AbmUserController::~AbmUserController() {
	delete abmService;
}
