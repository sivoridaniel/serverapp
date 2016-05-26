/*
 * ChatController.h
 *
 *  Created on: May 20, 2016
 *      Author: agustin
 */

#ifndef CHATCONTROLLER_H_
#define CHATCONTROLLER_H_

#include "../service/impl/ChatService.h"
#include "AbstractController.h"
#include "../webserver/mongoose.h"
#include "../utils/UriParser.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>
#include <vector>

using namespace std;
using namespace log4cplus;

class ChatController : public AbstractController{

private:
	ChatService* chatService;
public:
	ChatController();
	virtual ~ChatController();
	/**
	 * Se pasa los parametros del mensaje (uri de llamada para realizar la petición deseada
	 * por el usuario mediante api rest) junto con la conexion de mongoose.
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @result string json result
	 */
	string connect(struct mg_connection *nc, struct http_message *hm);

private:

	string getMessages(struct mg_connection *nc, struct http_message *hm);
	string getNewMessages(struct mg_connection *nc, struct http_message *hm);
	string postMessage(struct mg_connection *nc, struct http_message *hm);
	string updateLastMessageSeen(struct mg_connection *nc, struct http_message *hm);

	vector<string> parseMessageRequest(string json);
	vector<string> parseLastViewRequest(string json);
	string createMessagesResponse(vector<Message*> messages);
};

#endif /* CHATCONTROLLER_H_ */
