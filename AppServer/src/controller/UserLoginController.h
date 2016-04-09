/*
 * UserProfileController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */
#include "../model/UserLogin.h"
#include "../service/AuthenticationService.h"
#include "AbstractController.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

#ifndef SRC_CONTROLLER_USERLOGINCONTROLLER_H_
#define SRC_CONTROLLER_USERLOGINCONTROLLER_H_

using namespace std;
using namespace log4cplus;

class UserLoginController: public AbstractController{
public:
	UserLoginController();
	string connect(struct mg_connection *nc, struct http_message *hm);
	virtual ~UserLoginController();
private:
	AuthenticationService* authService;
	string event_handler_new_user(struct mg_connection *nc, struct http_message *hm);
	string event_handler_get_user(struct mg_connection *nc, struct http_message *hm);
};

#endif /* SRC_CONTROLLER_USERLOGINCONTROLLER_H_ */
