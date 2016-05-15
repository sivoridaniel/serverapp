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

string SearchCandidatesService::getCandidates(string idUser){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("SearchCandidatesService"));

	list<UserProfile*> candidates = this->runSearchAlgorithm(idUser, 3000);
	string json = createResponseJson(candidates);

	//libero recursos;
	for (list<UserProfile*>::iterator it=candidates.begin(); it!=candidates.end(); ++it){
		UserProfile* candidate = *it;
		delete candidate;
	}
    return json;
}

list<UserProfile*> SearchCandidatesService::runSearchAlgorithm(string idUser, double maxDistance){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("SearchCandidatesService"));

	list<UserProfile*> users;
	list<UserProfile*> candidates;
	try{
		users = this->sharedService->getUsers();
		UserProfile* userProfile = this->sharedService->getUser(idUser);
		list<Interest*> interests = userProfile->getInterests();

		for (list<UserProfile*>::iterator it=users.begin(); it!=users.end(); ++it){

			UserProfile* user = *it;
			list<Interest*> userInterests = user->getInterests();

			//filtro de distancia
			double distance = calculateDistance(user->getLocation(), userProfile->getLocation());

			if (distance > maxDistance){
				continue;
			}

			//filtro de intereses
			bool hasInterest = false;
			for (list<Interest*>::iterator itI = userInterests.begin(); itI!=userInterests.end(); ++itI ){
				Interest* userInterest = *itI;
				for (list<Interest*>::iterator itUI = interests.begin(); itUI!=interests.end(); ++itUI){
					Interest* myInterest = *itUI;
					if (myInterest->getCategory() == userInterest->getCategory() && myInterest->getValue() == userInterest->getValue()){
						hasInterest = true;
						break;
					}
				}
				if (hasInterest){
					break;
				}
			}

			if (hasInterest){
				candidates.push_back(user);
				it = users.erase(it);
			}
		}

		//libero recursos
		for (list<UserProfile*>::iterator it=users.begin(); it!=users.end(); ++it){
			UserProfile* user = *it;
			delete user;
		}
		delete userProfile;

	}
	catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}
	catch(exception& e)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}

	return candidates;
}

double SearchCandidatesService::calculateDistance(Location* location1, Location* location2){
	string lat1 = location1->getLatitude();
	string lon1 = location1->getLongitude();
	string lat2 = location2->getLatitude();
	string lon2 = location2->getLongitude();

	string::size_type sz;

	double la1 = stod(lat1, &sz);
	double lo1 = stod(lon1, &sz);
	double la2 = stod(lat2, &sz);
	double lo2 = stod(lon2, &sz);

	double distance = sqrt((la1 - la2)*(la1 - la2) + (lo1 - lo2)*(lo1 - lo2));

	return distance;
}

string SearchCandidatesService::createResponseJson(list<UserProfile*> candidates){


	for (list<UserProfile*>::iterator it=candidates.begin(); it!=candidates.end(); ++it){

	}
	return "";
}
