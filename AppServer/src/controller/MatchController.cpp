/*
 * MatchController.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#include "MatchController.h"

MatchController::MatchController() {
	matchService = new MatchService();
}

MatchController::~MatchController() {
	delete matchService;
}

string MatchController::connect(struct mg_connection *nc,
		struct http_message *hm) {

	if (mg_vcmp(&hm->uri, "/match/yes") == 0) {
		return event_handler_submit_yes(nc, hm);
	} else if (mg_vcmp(&hm->uri, "/match/no") == 0) {
		return event_handler_submit_no(nc, hm);
	} else if (mg_vcmp(&hm->uri, "/match/newmatches") == 0) {
			return event_handler_new_matches(nc, hm);
	} else if (mg_vcmp(&hm->uri, "/match/confirm") == 0) {
		return event_handler_confirm_match(nc, hm);
	}
	return ""; //Por default devuelve un JSON vacío.

}

string MatchController::event_handler_submit_yes(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));
	char idUser[100], idUserAccepted[100];

	/* Get form variables */
	mg_get_http_var(&hm->body, "idUser", idUser, sizeof(idUser));
	mg_get_http_var(&hm->body, "idUserAccepted", idUserAccepted, sizeof(idUserAccepted));

	/* Send headers */
	mg_printf(nc, "%s",
			"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Call match service */
	string sIdUser(idUser);
	string sIdUserAccepted(idUserAccepted);

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se agrega el usuario " << sIdUserAccepted << "a la lista de aceptados de " << sIdUser));
	string result = matchService->addToYesList(sIdUser, sIdUserAccepted);

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }", result.c_str());
	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

	return result;

}

string MatchController::event_handler_submit_no(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));
	char idUser[100], idUserRejected[100];

	/* Get form variables */
	mg_get_http_var(&hm->body, "idUser", idUser, sizeof(idUser));
	mg_get_http_var(&hm->body, "idUserRejected", idUserRejected, sizeof(idUserRejected));

	/* Send headers */
	mg_printf(nc, "%s",
			"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Call match service */
	string sIdUser(idUser);
	string sIdUserRejected(idUserRejected);

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se agrega el usuario " << sIdUserRejected << "a la lista de rechazados de " << sIdUser));
	string result = matchService->addToNoList(sIdUser, sIdUserRejected);

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }", result.c_str());
	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

	return result;
}


string MatchController::event_handler_confirm_match(struct mg_connection *nc,
		struct http_message *hm) {

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));
	char idUser[100], idUserConfirmed[100];

	/* Get form variables */
	mg_get_http_var(&hm->body, "idUser", idUser, sizeof(idUser));
	mg_get_http_var(&hm->body, "idUserConfirmed", idUserConfirmed, sizeof(idUserConfirmed));

	/* Send headers */
	mg_printf(nc, "%s",
			"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Call match service */
	string sIdUser(idUser);
	string sIdUserConfirmed(idUserConfirmed);

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se confirma el usuario " << sIdUserConfirmed << " por parte del usuario " << sIdUser));
	string result = matchService->confirmUser(sIdUser, sIdUserConfirmed);

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }", result.c_str());
	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

	return result;
}

string MatchController::event_handler_new_matches(struct mg_connection *nc, struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));
	char idUser[100];

	/* Get form variables */
	mg_get_http_var(&hm->body, "idUser", idUser, sizeof(idUser));

	/* Send headers */
	mg_printf(nc, "%s",
			"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Call match service */
	string sIdUser(idUser);

	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se obtienen los nuevos matches del usuario " << sIdUser));
	string result = matchService->getNewMatches(sIdUser);

	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "{ \"result\": \"%s\" }", result.c_str());
	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

	return result;
}

