/*
 * FactoryController.cpp
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "FactoryController.h"

void FactoryController::createControllers(string url)
{
	abmUserController = new AbmUserController(url);
	matchController = new MatchController(url);
	authenticationController = new AuthenticationController(url);
	searchController = new SearchCandidatesController(url);
	chatController = new ChatController(url);
}

void FactoryController::connect(struct mg_connection *nc, struct http_message *hm, struct mg_serve_http_opts s_http_server_opts)
{

	string res = abmUserController->connect(nc, hm, false);
	string not_found = abmUserController->STATUS_NOT_FOUND;

	if (res == not_found)
	{
		res = matchController->connect(nc, hm, false);
	}
	if (res == not_found)
	{
		res = authenticationController->connect(nc, hm, false);
	}
	if (res == not_found)
	{
		res = searchController->connect(nc, hm, false);
	}
	if (res == not_found)
	{
		res = chatController->connect(nc, hm, false);
	}

	mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */

}

FactoryController::~FactoryController()
{
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

