/*
 * UserProfileController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_CONTROLLER_ABMUSERCONTROLLER_H_
#define SRC_CONTROLLER_ABMUSERCONTROLLER_H_

#include "../model/UserLogin.h"
#include "../service/AbmUserService.h"
#include "AbstractController.h"
#include "../webserver/mongoose.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

class AbmUserController: public AbstractController{
public:
	AbmUserController();
	string connect(struct mg_connection *nc, struct http_message *hm);
	virtual ~AbmUserController();
private:
	AbmUserService* abmService;
	string event_handler_new_user(struct mg_connection *nc, struct http_message *hm);
	string event_handler_get_user(struct mg_connection *nc, struct http_message *hm);
};

#endif /* SRC_CONTROLLER_ABMUSERCONTROLLER_H_ */
