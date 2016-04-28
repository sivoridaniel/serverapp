/*
 * MatchController.h
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#ifndef MATCHCONTROLLER_H_
#define MATCHCONTROLLER_H_

#include "../service/MatchService.h"
#include "AbstractController.h"
#include "../model/NewMatchesResponse.h"
#include "../webserver/mongoose.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

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
	 * @result string json result
	 */
	string connect(struct mg_connection *nc, struct http_message *hm);

private:

	string event_handler_submit_yes(struct mg_connection *nc, struct http_message *hm);
	string event_handler_submit_no(struct mg_connection *nc, struct http_message *hm);
	string event_handler_new_matches(struct mg_connection *nc, struct http_message *hm);
	string event_handler_confirm_match(struct mg_connection *nc, struct http_message *hm);

};

#endif /* MATCHCONTROLLER_H_ */
