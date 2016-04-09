/*
 * FactoryController.cpp
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "FactoryController.h"

FactoryController* FactoryController::getInstance() {
	//createControllers();
	if(singletonFactoryCtrl == NULL){
		singletonFactoryCtrl = new FactoryController();
		//singletonFactoryCtrl->createControllers();
	}
	return singletonFactoryCtrl;
}

void FactoryController::createControllers(){
	userLoginController = new UserLoginController();
}

string FactoryController::connect(struct mg_connection *nc, struct http_message *hm){
	return userLoginController->connect(nc,hm);
}

FactoryController::~FactoryController() {
	userLoginController->~UserLoginController();
}

