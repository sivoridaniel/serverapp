/*
 * AbstractController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_CONTROLLER_ABSTRACTCONTROLLER_H_
#define SRC_CONTROLLER_ABSTRACTCONTROLLER_H_

#include <string>
#include "../webserver/mongoose.h"
#include "../utils/JwToken.h"
#include "../service/impl/AuthenticationService.h"
#include "../service/impl/AbmUserService.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace std;

class AbstractController {

public:

	string STATUS_NOK = "400 Bad Request";
	string STATUS_OK = "200 OK";
	string STATUS_NOT_FOUND = "404 Not Found";
	string STATUS_FORBIDDEN = "403 Forbidden";

	/**
	 * Método que se encarga de redireccionar al método según la uri.
	 * Devolverá el código de estado.
	 *
	 * 200: OK
	 * 400: ERROR ESPECIFICO
	 * 404: ERROR DESCONOCIDO / NO ENCUENTRA EL SERVICIO.
	 * 403: ACCESO NO AUTORIZADO
	 *
	 * @param struct mg_connection *nc
	 * @param struct http_message *hm
	 * @return string
	 */
	virtual string connect(struct mg_connection *nc, struct http_message *hm)=0;

	/**
	 * Método que verifica el token recibido cuando se consulta un servicio, en el cual
	 * se requiere estar logueado y no superar el tiempo máximo de request (60 segundos / 1 minuto).
	 * Pone en el cuerpo del mensaje (body) {"success": "true", "data":"logged"}
	 * Caso contrario pondrá en el body {"success": "false", "data":"mensaje de error"}
	 *
	 * Devuelve el código de estado.
	 * En caso de ser ok, renueva el token por 1 minuto más.
	 *
	 * 200: OK
	 * 400: TOKEN INVALIDO / BAD REQUEST
	 * 404: ENTITY NOT FOUND
	 *
	 * @param struct mg_connection *nc
	 * @param struct http_message *hm
	 * @return string
	 */
	virtual string valid_session(struct mg_connection *nc,
			struct http_message *hm) {

		Logger logger = Logger::getInstance(
				LOG4CPLUS_TEXT("AbstractController"));
		string ret_json = "";
		string code = "";

		string token = this->isLogged(nc, hm);

		if (!token.empty()) {
			ret_json = "{\"success\": \"true\", \"data\":\"logged\"}";
			code = STATUS_OK;
		} else {
			ret_json = "{\"success\": \"false\", \"data\":\"forbidden\"}";
			code = STATUS_NOK;
		}

		this->sendResponse(nc, code, ret_json, token);

		return code;
	}

	virtual void sendResponse(struct mg_connection *nc, string code,
			string json, string token) {

		const char* charcode = code.c_str();
		const char* buf = json.c_str();
		const char* tok = token.c_str();
		int lenght = (int) json.length();

		mg_printf(nc,
				"HTTP/1.1 %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\nToken: %s\r\n\r\n%s",
				charcode, lenght, tok, buf);

		nc->flags |= MG_F_SEND_AND_CLOSE;
	}

	virtual void sendChunkedResponse(struct mg_connection *nc, string code,
			string json, string token) {

		string transferEncoding = "chunked";
		string contentType = "application/json; charset=UTF-8";
		string headers = "HTTP/1.1 " + code + "\r\nTransfer-Encoding: "
				+ transferEncoding + "\r\nContent-Type: " + contentType
				+ "\r\nToken: " + token + "\r\n\r\n";

		/* Send headers */
		mg_printf(nc, "%s", headers.c_str());
		/* Send result back as a JSON object */
		mg_printf_http_chunk(nc, "%s", json.c_str());
		/* Send empty chunk, the end of response */
		mg_send_http_chunk(nc, "", 0);
	}

	virtual string getGenericJson(string success, string data) {
		string json = "";
		json = "{ \"success\": \"" + success + "\", \"data\": \"" + data
				+ "\"}";
		return json;
	}

	virtual string isLogged(struct mg_connection *nc, struct http_message *hm) {

		Logger logger = Logger::getInstance(
				LOG4CPLUS_TEXT("AbstractController"));

		UserProfile* userProfile = NULL;
		AuthenticationService * authenticationService = new AuthenticationService();
		AbmUserService * abmUserService = new AbmUserService();
		string nuevoToken = "";

		try {

			mg_str* mg_token = mg_get_http_header(hm, "token");
			if (mg_token == NULL){
				LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("NO HAY TOKEN EN EL HEADER"));
				delete authenticationService;
				delete abmUserService;
				return nuevoToken; //vacio
			}
			string token = string(mg_token->p, mg_token->len);

			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN RECIBIDO: "+token));

			bool isTokenValid = JwToken::isTokenValid(token);

			if (isTokenValid) {
				LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("TOKEN VALIDO"));
				string email = JwToken::getEmail(token);
				nuevoToken = JwToken::generarToken(email); //se genera el nuevo token, con el nuevo timestamp, renovando la sesion
				userProfile = authenticationService->getUserLogin(email, ""); //se pasa el password en vacío
				userProfile->setToken(nuevoToken);
				abmUserService->updateToken(userProfile); //se modifica el usuario para asignarle el nuevo token
				delete userProfile;
			} else {
				LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("TOKEN INVALIDO"));
			}

		} catch (exception & e) {
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		}
		delete abmUserService;
		delete authenticationService;
		return nuevoToken;
	}

	virtual ~AbstractController() {
	};
};

#endif /* SRC_CONTROLLER_ABSTRACTCONTROLLER_H_ */
