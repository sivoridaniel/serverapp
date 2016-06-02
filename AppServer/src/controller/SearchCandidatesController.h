/*
 * SearchCandidatesController.h
 *
 *  Created on: May 12, 2016
 *      Author: agustin
 */

#ifndef SEARCHCANDIDATESCONTROLLER_H_
#define SEARCHCANDIDATESCONTROLLER_H_

#include "../service/impl/SearchCandidatesService.h"
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
	/**
	 * Se pasa por parametro el id del usuario para el cual se quieren buscar los candidatos.
	 * Se aplica el algoritmo de busqueda para buscar candidatos cercanos y que no se hayan matcheado
	 * o rechazado antes.
	 *
	 * Devuelve el status:
	 *
	 * 200 OK
	 * 400 NOK
	 * 404 NOT_FOUND
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @result string
	 *
	 */
	string event_handler_search_candidates(struct mg_connection *nc, struct http_message *hm);
	string createSearchResponse(list<UserProfile*> candidates);


};

#endif /* SEARCHCANDIDATESCONTROLLER_H_ */
