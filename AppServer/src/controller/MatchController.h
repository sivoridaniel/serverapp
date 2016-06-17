/*
 * MatchController.h
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#ifndef MATCHCONTROLLER_H_
#define MATCHCONTROLLER_H_

#include "../service/impl/MatchService.h"
#include "AbstractController.h"
#include "../webserver/mongoose.h"
#include "../utils/UriParser.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>
#include <vector>

using namespace std;
using namespace log4cplus;
/**
 * Para manejar la api rest de matching
 */
class MatchController : public AbstractController{

private:
	MatchService* matchService;
public:
	MatchController();
	virtual ~MatchController();
	/**
	 * Se pasa los parametros del mensaje (uri de llamada para realizar la petición deseada
	 * por el usuario mediante api rest) junto con la conexion de mongoose.
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @return string json result
	 */
	string connect(struct mg_connection *nc, struct http_message *hm);

private:

	/**
	 * Se obtienen los parametros idFrom y idTo de la query, y se trata de agregar el usuario de
	 * idTo a la lista de aceptados de idFrom.
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
	 *
	 */
	string event_handler_submit_yes(struct mg_connection *nc, struct http_message *hm, string token);
	/**
	 * Se obtienen los parametros idFrom y idTo de la query, y se trata de agregar el usuario de
	 * idTo a la lista de rechazados de idFrom.
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
	 *
	 */
	string event_handler_submit_no(struct mg_connection *nc, struct http_message *hm, string token);
	/**
	 * Se obtiene el parametro id de la query, y se buscan los nuevos matchs de ese usuario.
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
	 *
	 */
	string event_handler_new_matches(struct mg_connection *nc, struct http_message *hm, string token);

	/**
	 * Se obtiene el parametro id de la query, y se buscan los chats de ese usuario.
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
	 *
	 */
	string event_handler_chats(struct mg_connection *nc, struct http_message *hm, string token);

	/**
	 * Se obtienen los parametros idTo y idFrom de la query, y se trata de agregar el idTo a la lista
	 * de matchs confirmados del usuario con idFrom.
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
	 *
	 */
	string event_handler_confirm_match(struct mg_connection *nc, struct http_message *hm, string token);
	/**
	 * Devuelve un vector de string con los parametros contenidos en el json. El vector de string
	 * tendrá en la primera posicion el idFrom y en la segunda el idTo, ids de usuarios con los
	 * que se van a hacer el match.
	 *
	 * @param json
	 * @throw JsonParseException
	 * @return vector<string>
	 */
	vector<string> parseMatchRequest(string json);
	/**
	 * Devuelve el json con los usuarios de nuevos matches.
	 *
	 * @param newMatches
	 * @return string
	 *
	 */
	string createNewMatchesResponse(list<UserProfile*> newMatches);

};

#endif /* MATCHCONTROLLER_H_ */
