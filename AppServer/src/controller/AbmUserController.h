/*
 * UserProfileController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_CONTROLLER_ABMUSERCONTROLLER_H_
#define SRC_CONTROLLER_ABMUSERCONTROLLER_H_

#include "../service/impl/AbmUserService.h"
#include "AbstractController.h"
#include "../webserver/mongoose.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

/**
 * Clase que maneja las peticiones del usuario por api rest
 * de creación de nuevo usuario, y obtención de un usuario en particular.
 */
class AbmUserController: public AbstractController{
public:
	AbmUserController();
	/**
	 * Se maneja la uri de consulta o creación de nuevo usuario.
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @result string json result
	 */
	string connect(struct mg_connection *nc, struct http_message *hm);
	virtual ~AbmUserController();
private:
	AbmUserService* abmService;
	string event_handler_new_user(struct mg_connection *nc, struct http_message *hm);
	string event_handler_update_user(struct mg_connection *nc, struct http_message *hm);
};

#endif /* SRC_CONTROLLER_ABMUSERCONTROLLER_H_ */
