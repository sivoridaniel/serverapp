/*
 * IAuthenticationController.h
 *
 * Interface para mockear
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */
#include <string>

#ifndef SRC_CONTROLLER_IAUTHENTICATIONCONTROLLER_H_
#define SRC_CONTROLLER_IAUTHENTICATIONCONTROLLER_H_

using namespace std;

class IAuthenticationController {
public:
	virtual bool isRegisterUser(string username, string password)=0;
	virtual ~IAuthenticationController(){};
};



#endif /* SRC_CONTROLLER_IAUTHENTICATIONCONTROLLER_H_ */
