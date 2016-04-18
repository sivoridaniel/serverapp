/*
 * AuthenticationController.cpp
 *
 *  Created on: 16 de abr. de 2016
 *      Author: pablo
 */

#include "AuthenticationController.h"

AuthenticationController::AuthenticationController() {
	// TODO Auto-generated constructor stub

}

bool AuthenticationController::isRegisterUser(string username, string password){
	if(username.compare("psivori")==0 && password.compare("1234")==0){
		return true;
	}
	return false;
}

AuthenticationController::~AuthenticationController() {
	// TODO Auto-generated destructor stub
}

