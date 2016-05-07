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

