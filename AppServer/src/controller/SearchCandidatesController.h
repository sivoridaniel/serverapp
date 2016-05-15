/*
 * SearchCandidatesController.h
 *
 *  Created on: May 12, 2016
 *      Author: agustin
 */

#ifndef SEARCHCANDIDATESCONTROLLER_H_
#define SEARCHCANDIDATESCONTROLLER_H_

#include "../service/SearchCandidatesService.h"
#include "AbstractController.h"
#include "../webserver/mongoose.h"
#include "../utils/UriParser.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

class SearchCandidatesController : public AbstractController{
private:
	SearchCandidatesService* searchService;
public:
	SearchCandidatesController();
	virtual ~SearchCandidatesController();
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

	string event_handler_search_candidates(struct mg_connection *nc, struct http_message *hm);

};

#endif /* SEARCHCANDIDATESCONTROLLER_H_ */