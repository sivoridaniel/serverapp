/*
 * IAuthenticationController.h
 *
 * Interface para mockear
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */
#include <string>
#include "../service/AuthenticationService.h"
#include "../service/AbmUserService.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "../utils/JwToken.h"


#ifndef SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_
#define SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_

using namespace std;
using namespace log4cplus;

class AuthenticationController {

private:
	AuthenticationService* authenticationService;
	AbmUserService* abmService;
	JwToken* jwToken;
	string generarToken(string username);

public:
	AuthenticationController();
	string login(string username, string password);
	virtual ~AuthenticationController();
};



#endif /* SRC_CONTROLLER_IAUTHENTICATIONCONTROLLER_H_ */
