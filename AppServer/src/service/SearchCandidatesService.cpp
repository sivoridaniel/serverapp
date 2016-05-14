/*
 * SearchCandidatesService.cpp
 *
 *  Created on: May 7, 2016
 *      Author: agustin
 */

#include "SearchCandidatesService.h"

SearchCandidatesService::SearchCandidatesService() {
	this->matchDao = new MatchDao();
	this->chatDao = new ChatDao();
	this->sharedService = new RemoteSharedService("http://shared-server-match.herokuapp.com");
}

SearchCandidatesService::SearchCandidatesService(MatchDao* matchDao, ChatDao* chatDao){
	this->matchDao = matchDao;
	this->chatDao = chatDao;
	this->sharedService = new RemoteSharedService("http://shared-server-match.herokuapp.com");

}


SearchCandidatesService::~SearchCandidatesService() {
	delete matchDao;
	delete chatDao;
	delete sharedService;
}

list<UserProfile*> SearchCandidatesService::getCandidates(string idUser){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("SearchCandidatesService"));

	Interest* interest = new Interest("cars", "Ford");
	try{
		this->sharedService->createInterest(interest);
	}catch(exception& e)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
	}

	list<Interest*> listInters = this->sharedService->getInterests();

    UserProfile* user = this->sharedService->getUser("3");

    user->addInterest(interest);
    this->sharedService->updateUser(user);

    list<UserProfile*> listaUsuarios = this->sharedService->getUsers();
    return listaUsuarios;
}


