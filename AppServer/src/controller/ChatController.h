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
	IChatService* chatService;
public:
	ChatController();
	ChatController(IChatService* chatService);
	virtual ~ChatController();
	/**
	 * Se pasa los parametros del mensaje (uri de llamada para realizar la petición deseada
	 * por el usuario mediante api rest) junto con la conexion de mongoose.
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @param bool test
	 * @return string json result
	 */
	string connect(struct mg_connection *nc, struct http_message *hm, bool test);

private:
	/**
	 * Se obtiene de la query los parametros idUser1 y idUser2 con los que se establecio el chat.
	 * Luego se obtienen todos los mensajes que hubo entre estos dos usuarios.
	 *
	 * Devuelve el status:
	 *
	 * 200 OK
	 * 400 NOK
	 * 404 NOT_FOUND
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @return string
	 */
	string getMessages(struct mg_connection *nc, struct http_message *hm, string token);
	/**
	 * Se obtienen de la query los parametros idUser1 y idUser2 con los que se establecio el chat.
	 * Luego se obtienen todos los mensajes nuevos que hubo entre estos dos usuarios.
	 *
	 * Devuelve el status:
	 *
	 * 200 OK
	 * 400 NOK
	 * 404 NOT_FOUND
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @return string
	 */
	string getNewMessages(struct mg_connection *nc, struct http_message *hm, string token);
	/**
	 * Se obtienen de la query los parametros idTo y idFrom.
	 * Luego se intenta postear un mensaje del idTo al idFrom.
	 *
	 * Devuelve el status:
	 *
	 * 200 OK
	 * 400 NOK
	 * 404 NOT_FOUND
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @return string
	 */
	string postMessage(struct mg_connection *nc, struct http_message *hm, string token);
	/**
	 * Se obtienen de la query los parametros idTo y idFrom.
	 * Se actualiza el ultimo mensaje visto por idFrom de idTo.
	 *
	 * Devuelve el status:
	 *
	 * 200 OK
	 * 400 NOK
	 * 404 NOT_FOUND
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @return string
	 */
	string updateLastMessageSeen(struct mg_connection *nc, struct http_message *hm, string token);
	/**
	 * Se procesa el json poniendo en el vector el idFrom, luego el idTo y por último
	 * el mensaje enviado por idTo al idFrom.
	 *
	 * @param json
	 * @throw JsonParseException
	 * @return vector<string>
	 */
	vector<string> parseMessageRequest(string json);
	/**
	 * Se procesa el json poniendo en el vector el idFrom, luego el idTo y por último
	 * el último mensaje visto por idFrom, el cual fue enviado por idTo.
	 *
	 * @param json
	 * @throw JsonParseException
	 * @return vector<string>
	 *
	 */
	vector<string> parseLastViewRequest(string json);
	/**
	 * Crea los nuevos mensajes enviado por id1 a id2, en formato json.
	 * Finalmente se retorna dicho json.
	 *
	 * @param messages
	 * @return string
	 */
	string createMessagesResponse(vector<Message*> messages);
};

#endif /* CHATCONTROLLER_H_ */
