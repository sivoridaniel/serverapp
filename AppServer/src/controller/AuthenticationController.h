/*
 * IAuthenticationController.h
 *
 * Interface para mockear
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */
#include <string>
#include "../service/impl/AuthenticationService.h"
#include "../service/impl/AbmUserService.h"
#include "../utils/JwToken.h"
#include "../webserver/mongoose.h"
#include "AbstractController.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#ifndef SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_
#define SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_

using namespace std;
using namespace log4cplus;

/**
 * Maneja la api rest de authenticacion.
 */
class AuthenticationController:public AbstractController {

public:
	AuthenticationController();
	/**
	 * Maneja las llamadas de login pasandole el message con los paramatros
	 * necesarios para el logueo.
	 *
	 * @param mg_connection
	 * @param http_message
	 * @return string json result (status ok o nok).
	 */
	string connect(struct mg_connection *nc, struct http_message *hm);
	virtual ~AuthenticationController();

private:
	AuthenticationService* authenticationService;
	AbmUserService* abmUserService;
	string event_handler_login_user(struct mg_connection *nc, struct http_message *hm);
	string event_handler_valid_session(struct mg_connection *nc, struct http_message *hm);
};



#endif /* SRC_CONTROLLER_IAUTHENTICATIONCONTROLLER_H_ */
