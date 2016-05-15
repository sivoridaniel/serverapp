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
		if (mg_vcmp(&hm->method, "POST") == 0) {
			return event_handler_submit_yes(nc, hm);
		}
	} else if (mg_vcmp(&hm->uri, "/match/no") == 0) {
		if (mg_vcmp(&hm->method, "POST") == 0) {
			return event_handler_submit_no(nc, hm);
		}
	} else if (mg_vcmp(&hm->uri, "/match") == 0) {
		if (mg_vcmp(&hm->method, "GET") == 0) {
			return event_handler_new_matches(nc, hm);
		}
	} else if (mg_vcmp(&hm->uri, "/match/confirm") == 0) {
		if (mg_vcmp(&hm->method, "POST") == 0) {
			return event_handler_confirm_match(nc, hm);
		}
	}
	return ""; //Por default devuelve un JSON vacío.

}

string MatchController::event_handler_submit_yes(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));

	string json = "";
	string code = "";
	string body = string((&hm->body)->p, (&hm->body)->len);

	try {
		vector<string> params = parseMatchRequest(body);
		string idFrom = params[0];
		string idTo = params[1];

		/* Call match service */
		LOG4CPLUS_DEBUG(logger,
				LOG4CPLUS_TEXT("Se intenta agregar el usuario " << idTo << " a la lista de aceptados de " << idFrom));

		try {
			bool matched = matchService->addToYesList(idFrom, idTo);
			if (matched) {
				LOG4CPLUS_INFO(logger,
						"se produzco un match entre "<<idFrom<<" y "<<idTo);
				json = "{ \"success\": \"true\", \"data\": \"matched\"}";
			} else {
				LOG4CPLUS_INFO(logger,
						"se agrega el usuario "<<idTo<<" a la lista de aceptados de "<<idFrom);
				json = "{ \"success\": \"true\" \"data\": \"unmatched\"}";
			}
		} catch (EntityExistsException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json =
					"{ \"success\": \"false\", \"data\": \"El usuario ya existe en la lista de aceptados\"}";
		} catch (EntityNotFoundException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "404";
			json =
					"{ \"success\": \"false\", \"data\": \"Uno de los usuarios no existe en la base\"}";
		} catch (exception& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json = "{ \"success\": \"false\", \"data\": \"Error desconocido\"}";
		}

	} catch (JsonParseException& e) {
		code = "400";
		json =
				"{ \"success\": \"false\", \"data\": \"Bad Request: formato incorrecto de json\"}";
	}

	string headers = "HTTP/1.1 " + code
			+ " OK\r\nTransfer-Encoding: chunked\r\n\r\n";

	/* Send headers */
	mg_printf(nc, "%s", headers.c_str());
	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", json.c_str());
	/* Send empty chunk, the end of response */
	mg_send_http_chunk(nc, "", 0);

	return json;

}

string MatchController::event_handler_submit_no(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));

	string json = "";
	string code = "";
	string body = string((&hm->body)->p, (&hm->body)->len);

	try {
		vector<string> params = parseMatchRequest(body);
		string idFrom = params[0];
		string idTo = params[1];

		/* Call match service */
		LOG4CPLUS_DEBUG(logger,
				LOG4CPLUS_TEXT("Se intenta agregar el usuario " << idTo << " a la lista de rechazados de " << idFrom));

		try {
			matchService->addToNoList(idFrom, idTo);
			LOG4CPLUS_INFO(logger,
					"se agrega el usuario "<<idTo<<" a la lista de rechazados de "<<idFrom);
			json = "{ \"success\": \"true\" \"data\": \"rejected\"}";
		} catch (EntityExistsException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json =
					"{ \"success\": \"false\", \"data\": \"El usuario ya existe en la lista de rechazados\"}";
		} catch (EntityNotFoundException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "404";
			json =
					"{ \"success\": \"false\", \"data\": \"Uno de los usuarios no existe en la base\"}";
		} catch (exception& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json = "{ \"success\": \"false\", \"data\": \"Error desconocido\"}";
		}

	} catch (JsonParseException& e) {
		code = "400";
		json =
				"{ \"success\": \"false\", \"data\": \"Bad Request: formato incorrecto de json\"}";
	}

	string headers = "HTTP/1.1 " + code
			+ " OK\r\nTransfer-Encoding: chunked\r\n\r\n";

	/* Send headers */
	mg_printf(nc, "%s", headers.c_str());
	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", json.c_str());
	/* Send empty chunk, the end of response */
	mg_send_http_chunk(nc, "", 0);

	return json;
}

string MatchController::event_handler_confirm_match(struct mg_connection *nc,
		struct http_message *hm) {

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));

	string json = "";
	string code = "";
	string body = string((&hm->body)->p, (&hm->body)->len);

	try {
		vector<string> params = parseMatchRequest(body);
		string idFrom = params[0];
		string idTo = params[1];

		/* Call match service */
		LOG4CPLUS_DEBUG(logger,
				LOG4CPLUS_TEXT("Se intenta agregar el usuario " << idTo << " a la lista de confirmados de " << idFrom));

		try {
			matchService->confirmUser(idFrom, idTo);
			LOG4CPLUS_INFO(logger,
					"se agrega el usuario "<<idTo<<" a la lista de confirmados de "<<idFrom);
			json = "{ \"success\": \"true\" \"data\": \"confirmed\"}";
		} catch (EntityExistsException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json =
					"{ \"success\": \"false\", \"data\": \"El usuario ya existe en la lista de confirmados\"}";
		} catch (EntityNotFoundException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "404";
			json =
					"{ \"success\": \"false\", \"data\": \"Uno de los usuarios no existe en la base\"}";
		} catch (exception& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json = "{ \"success\": \"false\", \"data\": \"Error desconocido\"}";
		}

	} catch (JsonParseException& e) {
		code = "400";
		json =
				"{ \"success\": \"false\", \"data\": \"Bad Request: formato incorrecto de json\"}";
	}

	string headers = "HTTP/1.1 " + code
			+ " OK\r\nTransfer-Encoding: chunked\r\n\r\n";

	/* Send headers */
	mg_printf(nc, "%s", headers.c_str());
	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", json.c_str());
	/* Send empty chunk, the end of response */
	mg_send_http_chunk(nc, "", 0);

	return json;
}

string MatchController::event_handler_new_matches(struct mg_connection *nc,
		struct http_message *hm) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchController"));

	string json = "";
	string code = "";

	string query = string((&hm->query_string)->p, (&hm->query_string)->len);
	string id = UriParser::getId(query);

	if (id.compare("") == 0) {
		code = "400";
		json = "{ \"success\": \"false\", \"data\": \"Bad Request\"}";
	} else {

		LOG4CPLUS_DEBUG(logger,
				LOG4CPLUS_TEXT("Se obtienen los nuevos matches del usuario " << id));

		try {
			list<string> matches = matchService->getNewMatches(id);
			NewMatchesResponse* response = new NewMatchesResponse(matches);
			string resp = response->toJson();
			json = resp;
			delete response;
		} catch (EntityNotFoundException& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "404";
			json =
					"{ \"success\": \"false\", \"data\": \"Uno de los usuarios no existe en la base\"}";
		} catch (exception& e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = "400";
			json = "{ \"success\": \"false\", \"data\": \"Error desconocido\"}";
		}
	}

	string headers = "HTTP/1.1 " + code
			+ " OK\r\nTransfer-Encoding: chunked\r\n\r\n";

	/* Send headers */
	mg_printf(nc, "%s", headers.c_str());
	/* Send result back as a JSON object */
	mg_printf_http_chunk(nc, "%s", json.c_str());
	/* Send empty chunk, the end of response */
	mg_send_http_chunk(nc, "", 0);

	return json;
}

vector<string> MatchController::parseMatchRequest(string json) {
	Json::Value root;
	Json::Reader reader;
	vector<string> params;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
		throw JsonParseException();
	}
	string idFrom = root.get("idFrom", "").asString();
	string idTo = root.get("idTo", "").asString();
	if (idFrom.empty() || idTo.empty()) {
		throw JsonParseException();
	}
	params.push_back(idFrom);
	params.push_back(idTo);
	return params;
}

