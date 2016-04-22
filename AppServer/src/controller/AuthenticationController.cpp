/*
 * AuthenticationController.cpp
 *
 *  Created on: 16 de abr. de 2016
 *      Author: pablo
 */

#include "IAuthenticationController.h"

class AuthenticationController: public IAuthenticationController{

AuthenticationController() {
	// TODO Auto-generated constructor stub

}

virtual bool isRegisterUser(string username, string password){
	return true;
}

virtual ~AuthenticationController() {
	// TODO Auto-generated destructor stub
}

};
