/*
 * SearchCandidatesController.cpp
 *
 *  Created on: May 12, 2016
 *      Author: agustin
 */

#include "SearchCandidatesController.h"

SearchCandidatesController::SearchCandidatesController() {
	searchService = new SearchCandidatesService();
}

SearchCandidatesController::~SearchCandidatesController() {
	delete searchService;
}

string SearchCandidatesController::connect(struct mg_connection *nc,
		struct http_message *hm) {

	if (mg_vcmp(&hm->uri, "/candidates") == 0) {
		return event_handler_search_candidates(nc, hm);
	}
	return ""; //Por default devuelve un JSON vacío.

}

string SearchCandidatesController::event_handler_search_candidates(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));
	char idUser[100], idUserAccepted[100];

	/* Get form variables */
	//mg_get_http_var(&hm->body, "idUser", idUser, sizeof(idUser));

	/* Send headers */
	mg_printf(nc, "%s",
			"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Call match service */
	string sIdUser("alinari");

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se buscan candidatos para el usuario " << sIdUser));
	string json = "{ \"result\": \"failed\" }";

	try{
		list<UserProfile*> listCandidates = searchService->getCandidates(sIdUser);
		json = "{ \"result\": \"success\" }";

	}catch(exception& e)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		json = "{ \"result\": \"failed\" }";
	}

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", json.c_str());
	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

	return json;

}


