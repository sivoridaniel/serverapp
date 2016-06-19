/*
 * ChatController.cpp
 *
 *  Created on: May 20, 2016
 *      Author: agustin
 */

#include "ChatController.h"

ChatController::ChatController() {
	this->chatService = new ChatService();

}

ChatController::ChatController(IChatService* chatService){
	this->chatService = chatService;
}


ChatController::~ChatController() {
	delete chatService;
}

string ChatController::connect(struct mg_connection *nc,
		struct http_message *hm, bool test) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatController"));

	if (mg_vcmp(&hm->uri, "/chat") == 0) {
		if (mg_vcmp(&hm->method, "GET") == 0) {
			string token = isLogged(hm, test);
			if (!token.empty()){
				return getMessages(nc, hm, token);
			}else{
				return sendForbiddenResponse(nc);
			}
		}
	} else if (mg_vcmp(&hm->uri, "/chat/new") == 0) {
		if (mg_vcmp(&hm->method, "GET") == 0) {
			string token = isLogged(hm, test);
			if (!token.empty()){
				return getNewMessages(nc, hm, token);
			}else{
				return sendForbiddenResponse(nc);
			}
		}
	} else if (mg_vcmp(&hm->uri, "/chat/message") == 0) {
		if (mg_vcmp(&hm->method, "POST") == 0) {
			string token = isLogged(hm, test);
			if (!token.empty()){
				return postMessage(nc, hm, token);
			}else{
				return sendForbiddenResponse(nc);
			}
		}
	} else if (mg_vcmp(&hm->uri, "/chat/last") == 0) {
		if (mg_vcmp(&hm->method, "PUT") == 0) {
			string token = isLogged(hm, test);
			if (!token.empty()){
				return updateLastMessageSeen(nc, hm, token);
			}else{
				return sendForbiddenResponse(nc);
			}
		}
	}
	return STATUS_NOT_FOUND; //Por default devuelve un JSON vacío.

}

string ChatController::getMessages(struct mg_connection *nc, struct http_message *hm, string token){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatController"));

	string json = "";
	string code = "";

	string query = string((&hm->query_string)->p, (&hm->query_string)->len);
    string idUser1 = "";
    string idUser2 = "";

	vector<string> params = UriParser::getParams(query);
	if (params.size()!=2){
		code = STATUS_NOK;
		json = this->getGenericJson("false","invalid params");
	}
	else{
		idUser1 = params[0];
	 	idUser2 = params[1];
	}

	if (idUser1.compare("") == 0 || idUser2.compare("") == 0) {
		code = STATUS_NOK;
		json = this->getGenericJson("false","invalid user in params");
	} else {
		try {
			vector<Message*> messages = chatService->getAllMessages(idUser1,idUser2);
			LOG4CPLUS_INFO(logger, "se obtiene la conversacion entre "<<idUser1<<" y "<<idUser2);
			json = createMessagesResponse(messages);
			//libero recursos
			for (vector< Message* >::iterator it=messages.begin(); it!=messages.end(); ++it){
				Message* message = *it;
				delete message;
			}
			code = STATUS_OK;
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

string ChatController::getNewMessages(struct mg_connection *nc, struct http_message *hm, string token){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatController"));

	string json = "";
	string code = "";

	string query = string((&hm->query_string)->p, (&hm->query_string)->len);
    string idUser1 = "";
    string idUser2 = "";

	vector<string> params = UriParser::getParams(query);
	if (params.size()!=2){
		code = STATUS_NOK;
		json = this->getGenericJson("false","invalid params");
	}
	else{
		idUser1 = params[0];
	 	idUser2 = params[1];
	}

	if (idUser1.compare("") == 0 || idUser2.compare("") == 0) {
		code = STATUS_NOK;
		json = this->getGenericJson("false","invalid user in params");
	} else {
		try {
			vector<Message*> messages = chatService->getNewMessages(idUser1,idUser2);
			LOG4CPLUS_INFO(logger, "se obtiene los nuevos mensajes para "<<idUser1<<" de "<<idUser2);
			json = createMessagesResponse(messages);
			//libero recursos
			for (vector< Message* >::iterator it=messages.begin(); it!=messages.end(); ++it){
				Message* message = *it;
				delete message;
			}
			code = STATUS_OK;
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

string ChatController::postMessage(struct mg_connection *nc, struct http_message *hm, string token){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatController"));

	string json = "";
	string code = "";
	string body = string((&hm->body)->p, (&hm->body)->len);

	try {
		vector<string> params = parseMessageRequest(body);
		string idFrom = params[0];
		string idTo = params[1];
		string message = params[2];

		/* Call match service */
		LOG4CPLUS_DEBUG(logger,
				LOG4CPLUS_TEXT("Se intenta postear un mensaje del usuario " << idTo << " a " << idFrom));

		try {
			chatService->addNewMessage(idFrom, idTo, message);

			LOG4CPLUS_INFO(logger, "se agrega el mensaje de "<<idTo<<" a la conversacion con "<<idFrom);
			code = STATUS_OK;
			json = this->getGenericJson("true","message posted");
		} catch (EntityNotFoundException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOT_FOUND;
			json = this->getGenericJson("false",e.what());
		} catch (exception& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false",e.what());
		}

	} catch (JsonParseException& e) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		code = STATUS_NOK;
		json = this->getGenericJson("false",e.what());
	}

	this->sendResponse(nc, code, json, token);

	return code;
}

string ChatController::updateLastMessageSeen(struct mg_connection *nc, struct http_message *hm, string token){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatController"));

	string json = "";
	string code = "";
	string body = string((&hm->body)->p, (&hm->body)->len);

	try {
		vector<string> params = parseLastViewRequest(body);
		string idFrom = params[0];
		string idTo = params[1];
		long idMessage = std::stol(params[2]);

		/* Call match service */
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se intenta actualizar el ultimo mensaje de "<<idTo<<" visto por usuario " << idFrom));

		try {
			chatService->updateLastMessageSeen(idFrom, idTo, idMessage);

			LOG4CPLUS_INFO(logger, "se actualiza el ultimo mensaje visto por "<<idFrom<<" de"<<idTo);
			json = this->getGenericJson("true","last message seen updated");
			code = STATUS_OK;

		} catch (EntityNotFoundException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOT_FOUND;
			json = this->getGenericJson("false",e.what());
		} catch (exception& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false",e.what());
		}

	} catch (JsonParseException& e) {
		code = STATUS_NOK;
		json = this->getGenericJson("false",e.what());
	}

	this->sendResponse(nc, code, json, token);


	return code;
}

vector<string> ChatController::parseMessageRequest(string json){
	Json::Value root;
	Json::Reader reader;
	vector<string> params;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
		throw JsonParseException();
	}
	string param1 = root.get("idFrom", "").asString();
	string param2 = root.get("idTo", "").asString();
	string param3 = root.get("message", "").asString();
	if (param1.empty() || param2.empty() || param3.empty()) {
		throw JsonParseException();
	}
	params.push_back(param1);
	params.push_back(param2);
	params.push_back(param3);
	return params;
}

vector<string> ChatController::parseLastViewRequest(string json){
	Json::Value root;
	Json::Reader reader;
	vector<string> params;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
		throw JsonParseException();
	}
	string param1 = root.get("idFrom", "").asString();
	string param2 = root.get("idTo", "").asString();
	string param3 = root.get("idMessage", "").asString();
	if (param1.empty() || param2.empty() || param3.empty()) {
		throw JsonParseException();
	}
	params.push_back(param1);
	params.push_back(param2);
	params.push_back(param3);
	return params;
}


string ChatController::createMessagesResponse(vector<Message*> messages){
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	if (messages.size()==0){
		return "{ \"chat\": []}";
	}

	int i = 0;
	for (vector< Message* >::iterator it=messages.begin(); it!=messages.end(); ++it){
		Message* message = *it;

		root["chat"][i]["message"]["id"] = std::to_string(message->getId());
		root["chat"][i]["message"]["user"] = message->getIdUser();
		root["chat"][i]["message"]["content"] = message->getMessage();

		i++;
	}

	string json = writer.write(root);
	return json;
}
