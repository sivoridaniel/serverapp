/*
 * AuthenticationController.h
 *
 *  Created on: 16 de abr. de 2016
 *      Author: pablo
 */
#include <string>

#ifndef SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_
#define SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_
using namespace std;

class AuthenticationController {
public:
	AuthenticationController();
	bool isRegisterUser(string username, string password);
	virtual ~AuthenticationController();
};

#endif /* SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_ */
