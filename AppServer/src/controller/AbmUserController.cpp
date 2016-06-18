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

AbmUserController::AbmUserController(IAbmUserService* abmService){
	this->abmService = abmService;
}


string AbmUserController::connect(struct mg_connection *nc, struct http_message *hm, bool test){

	if(mg_vcmp(&hm->uri, "/user/newuser") == 0){
		if (mg_vcmp(&hm->method, "POST") == 0) {
			return event_handler_new_user(nc,hm);
		}
	}else if(mg_vcmp(&hm->uri, "/user/updateuser") == 0){
		if (mg_vcmp(&hm->method, "PUT") == 0) {
			string token = isLogged(hm, test);
			if (!token.empty()){
				return event_handler_update_user(nc,hm,token);
			}else{
				return sendForbiddenResponse(nc);
			}
		}
	}else if(mg_vcmp(&hm->uri, "/user/photo") == 0){
		if (mg_vcmp(&hm->method, "GET") == 0) {
			string token = isLogged(hm, test);
			if (!token.empty()){
				return event_handler_get_photo(nc,hm, token);
			}else{
				return sendForbiddenResponse(nc);
			}
		}
	}else if(mg_vcmp(&hm->uri, "/interests") == 0){
		if (mg_vcmp(&hm->method, "GET") == 0) {
			string token = isLogged(hm, test);
			if (!token.empty()){
				return event_handler_get_interests(nc, token);
			}else{
				return sendForbiddenResponse(nc);
			}
		}
	}

	return STATUS_NOT_FOUND; //Por default devuelve NOT_FOUND.
}

string AbmUserController::event_handler_new_user(struct mg_connection *nc, struct http_message *hm){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserController"));

	string data = string(hm->body.p,hm->body.len);
	string json = "";
	string code = STATUS_OK;
	string messageCode = "";
	string token = "";
	UserProfile* userProfile=NULL;

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("JSon Ingresado: " << data));

	try{
		userProfile = new UserProfile(data);

		try{
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Guardando en db usuario " << userProfile->getName()));
		    token = JwToken::generarToken(userProfile->getEmail());
			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN: "+token));
			userProfile->setToken(token);
			abmService->createNewUser(userProfile);
			json = this->getGenericJson("true", userProfile->getId());
			code = STATUS_OK;
			delete userProfile;
		}catch(InvalidEntityException& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("true", e.what());
			delete userProfile;
		}catch(EntityExistsException& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(RemoteException& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}
		catch(exception& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}

	}catch(JsonParseException& e){
		code = STATUS_NOK;
		json = this->getGenericJson("false", e.what());
		delete userProfile;
	}

	this->sendResponse(nc,code,json,token);

	return code;
}

string AbmUserController::event_handler_update_user(struct mg_connection *nc, struct http_message *hm, string token){

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserController"));
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
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(EntityNotFoundException& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOT_FOUND;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(RemoteException& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}catch(exception& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
			delete userProfile;
		}


	}catch(JsonParseException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		code = STATUS_NOK;
		json = this->getGenericJson("false", e.what());
		delete userProfile;
	}

	this->sendResponse(nc,code,json,token);

	return code;
}

string AbmUserController::event_handler_get_interests(struct mg_connection *nc, string token){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserController"));
	string code = "";
	string json = "";

	try{
		list<Interest*> intereses = abmService->getInterests();

		json = createInterestsResponse(intereses);

		code = STATUS_OK;

		for (list< Interest* >::iterator it=intereses.begin(); it!=intereses.end(); ++it){
			Interest* interest = *it;
			delete interest;
		}
	}catch(exception& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		code = STATUS_NOK;
		json = this->getGenericJson("false", e.what());
	}

	this->sendResponse(nc,code,json, token);

	return code;
}

string AbmUserController::event_handler_get_photo(struct mg_connection *nc, struct http_message *hm, string token){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbmUserController"));

	string json = "";
	string code = "";
	string id = "";
	string query = string((&hm->query_string)->p, (&hm->query_string)->len);
	vector<string> params = UriParser::getParams(query);

	if (params.size()!=1){
		code = STATUS_NOK;
		json = this->getGenericJson("false","invalid params");
	}else{
		id = params[0];
	}

	if (id.compare("") == 0) {
		code = STATUS_NOK;
		json = this->getGenericJson("false","invalid user in params");
	} else {

		LOG4CPLUS_DEBUG(logger,
				LOG4CPLUS_TEXT("Se obtiene la foto del usuario " << id));

		try {
			string photo = abmService->getPhoto(id);
			code = STATUS_OK;
			json = createPhotoResponse(photo);
		} catch (EntityNotFoundException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOT_FOUND;
			json = this->getGenericJson("false",e.what());
		} catch (exception& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false",e.what());
		}
	}

	this->sendResponse(nc, code, json, token);


	return code;
}

string AbmUserController::createPhotoResponse(string photo){
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	root["photo"] = photo;
	string json = writer.write(root);
	return json;
}

string AbmUserController::createInterestsResponse(list<Interest*> intereses){

	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	if (intereses.size()==0){
		return "{ \"interests\": []}";
	}

	int i = 0;
	for (list< Interest* >::iterator it=intereses.begin(); it!=intereses.end(); ++it){
		Interest* interest = *it;

		root["interests"][i]["category"] = interest->getCategory();
		root["interests"][i]["value"] = interest->getValue();

		i++;
	}

	string json = writer.write(root);
	return json;

}

AbmUserController::~AbmUserController() {
	delete abmService;
}
