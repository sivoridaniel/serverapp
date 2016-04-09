/*
 * FactoryController.cpp
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "FactoryController.h"


void FactoryController::createControllers(){
	userLoginController = new UserLoginController();
}

string FactoryController::connect(struct mg_connection *nc, struct http_message *hm){
	return userLoginController->connect(nc,hm);
}

FactoryController::~FactoryController() {
	userLoginController->~UserLoginController();
}

FactoryController* FactoryController::singletonFactoryCtrl;
UserLoginController* FactoryController::userLoginController;
