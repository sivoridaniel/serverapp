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

	/**
	 * Método que se encarga de redireccionar al método según la uri.
	 * Devolverá el código de estado.
	 *
	 * 200: OK
	 * 400: ERROR ESPECIFICO
	 * 404: ERROR DESCONOCIDO / NO ENCUENTRA EL SERVICIO.
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
	virtual string valid_session(struct mg_connection *nc, struct http_message *hm){

		UserProfile* userProfile = NULL;
		AuthenticationService * authenticationService = new AuthenticationService();
		AbmUserService * abmUserService = new AbmUserService();
		Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbstractController"));
		string ret_json = "{\"success\": \"true\", \"data\":\"logged\"}";
		string code =STATUS_OK;
		try{

		mg_str* mg_token = mg_get_http_header(hm,"token");
		string token = string(mg_token->p,mg_token->len);

		/* Get Form variables */

		//mg_get_http_var(&hm->message, "token", vecToken, sizeof(vecToken));

		//mg_http_parse_header(&hm->mg_header,'token',vecToken,sizeof(vecToken));

		/* Call authentication service */


			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN RECIBIDO: "+token));

			bool isTokenValid = JwToken::isTokenValid(token);


			if(isTokenValid){
				LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("TOKEN VALIDO"));
				string email = JwToken::getEmail(token);
				token = JwToken::generarToken(email); //se genera el nuevo token, con el nuevo timestamp, renovando la sesion
				userProfile=authenticationService->getUserLogin(email,""); //se pasa el password en vacío
				userProfile->setToken(token);
				abmUserService->updateToken(userProfile); //se modifica el usuario para asignarle el nuevo token
				code=token;
				delete userProfile;
			}else{
				code=STATUS_NOK;
				ret_json = "{\"success\": \"false\", \"data\":\"Bad request\"}";
			}

		}catch(EntityNotFoundException & e){
			ret_json = (string("{\"success\": \"false\", \"data\":\"")+e.what()+string("\"}"));
			code=STATUS_NOT_FOUND;
			delete userProfile;
		}catch(exception & e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			ret_json = (string("{\"success\": \"false\", \"data\":\"")+e.what()+string("\"}"));
			code=STATUS_NOK;
			delete userProfile;
		}

		mg_printf_http_chunk(nc, "%s", ret_json.c_str());

		delete abmUserService;
		delete authenticationService;

		return code;
	}

	virtual void sendResponse(struct mg_connection *nc, string code, string json, string token){

		string transferEncoding = "chunked";
		string contentType = "application/json; charset=UTF-8";
		string headers = "HTTP/1.1 "+code+"\r\nTransfer-Encoding: "+transferEncoding+"\r\nContent-Type: "+contentType+"\r\nToken: "+token+"\r\n\r\n";

		/* Send headers */
		mg_printf(nc, "%s", headers.c_str());
		/* Send result back as a JSON object */
		mg_printf_http_chunk(nc, "%s", json.c_str());
		/* Send empty chunk, the end of response */
		mg_send_http_chunk(nc, "", 0);
	}

	virtual string getGenericJson(string success, string data){
		string json = "";
		json = "{ \"success\": \""+success+"\", \"data\": \""+data+"\"}";
		return json;
	}

	virtual ~AbstractController(){};
};

#endif /* SRC_CONTROLLER_ABSTRACTCONTROLLER_H_ */
