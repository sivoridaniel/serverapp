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

		if (mg_vcmp(&hm->method, "GET") == 0){
			return event_handler_search_candidates(nc, hm);
		}

	}
	return ""; //Por default devuelve un JSON vacío.

}

string SearchCandidatesController::event_handler_search_candidates(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));
	char idUser[100], idUserAccepted[100];

	string json = "";
	string code = "";
	string query = string((&hm->query_string)->p, (&hm->query_string)->len);
	string id = UriParser::getId(query);

	if (id.compare("") == 0){
		string code = "400";
		string json = "{ \"success\": \"false\", \"data\": \"Bad Request\"}";
	}
	else{
		/* Call match service */
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se buscan candidatos para el usuario " << id));
		try{
			json = searchService->getCandidates(id);
			code = "200";
		}
		catch(EntityNotFoundException& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			json = "{ \"success\": \"false\", \"data\": \"No existe el usuario con id "+id+"\"}";
			code = "404";
		}
		catch(exception& e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json = "{ \"success\": \"false\", \"data\": \"Bad Request\"}";
		}
	}

	string headers = "HTTP/1.1 "+code+" OK\r\nTransfer-Encoding: chunked\r\n\r\n";

	/* Send headers */
	mg_printf(nc, "%s", headers.c_str());
	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", json.c_str());
	/* Send empty chunk, the end of response */
	mg_send_http_chunk(nc, "", 0);

	return json;

}


