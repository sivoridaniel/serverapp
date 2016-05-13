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
}

SearchCandidatesService::SearchCandidatesService(MatchDao* matchDao, ChatDao* chatDao){
	this->matchDao = matchDao;
	this->chatDao = chatDao;
}


SearchCandidatesService::~SearchCandidatesService() {
	delete matchDao;
	delete chatDao;
}

list<UserProfile*> SearchCandidatesService::getCandidates(string idUser){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("SearchCandidatesService"));

	RestClient::response r = RestClient::get("http://shared-server-match.herokuapp.com/interests");
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Response body "<<r.body));


    list<UserProfile*> listaUsuarios;
    return listaUsuarios;
}


