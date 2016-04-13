/*
 * FactoryController.cpp
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "FactoryController.h"


void FactoryController::createControllers(){
	abmUserController = new AbmUserController();
	matchController = new MatchController();
}

void FactoryController::connect(struct mg_connection *nc, struct http_message *hm, struct mg_serve_http_opts s_http_server_opts){
	string res = abmUserController->connect(nc,hm);
	if (res.empty()){
		res = matchController->connect(nc,hm);
	}
	if (res.empty()){
        mg_serve_http(nc, hm, s_http_server_opts);  /* Serve static content */
	}
}

FactoryController::~FactoryController() {
	abmUserController->~AbmUserController();
	matchController->~MatchController();
}

FactoryController* FactoryController::singletonFactoryCtrl;
AbmUserController* FactoryController::abmUserController;
MatchController* FactoryController::matchController;

