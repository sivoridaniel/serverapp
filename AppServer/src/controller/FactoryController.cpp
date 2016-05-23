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
	authenticationController = new AuthenticationController();
	searchController = new SearchCandidatesController();
	chatController = new ChatController();
}

void FactoryController::connect(struct mg_connection *nc, struct http_message *hm, struct mg_serve_http_opts s_http_server_opts){

	string res = abmUserController->connect(nc,hm);
	string not_found = abmUserController->STATUS_NOT_FOUND;

	if (res==not_found){
		res = matchController->connect(nc,hm);
	}
	if(res==not_found){
		res = authenticationController->connect(nc,hm);
	}
	if (res==not_found){
		res = searchController->connect(nc,hm);
	}
	if (res==not_found){
		res = chatController->connect(nc,hm);
	}

        mg_serve_http(nc, hm, s_http_server_opts);  /* Serve static content */

}

FactoryController::~FactoryController() {
	delete abmUserController;
	delete matchController;
	delete authenticationController;
	delete searchController;
	delete chatController;
}

FactoryController* FactoryController::singletonFactoryCtrl;
AbmUserController* FactoryController::abmUserController;
MatchController* FactoryController::matchController;
AuthenticationController* FactoryController::authenticationController;
SearchCandidatesController* FactoryController::searchController;
ChatController* FactoryController::chatController;

