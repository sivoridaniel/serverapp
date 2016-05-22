/*
 * SearchCandidatesService.cpp
 *
 *  Created on: May 7, 2016
 *      Author: agustin
 */

#include "SearchCandidatesService.h"

SearchCandidatesService::SearchCandidatesService() {
	this->matchService = new MatchService();
	this->sharedService = new RemoteSharedService("http://shared-server-match.herokuapp.com");
}

SearchCandidatesService::SearchCandidatesService(MatchService* matchService){
	this->matchService = matchService;
	this->sharedService = new RemoteSharedService("http://shared-server-match.herokuapp.com");

}


SearchCandidatesService::~SearchCandidatesService() {
	delete matchService;
	delete sharedService;
}

list<UserProfile*> SearchCandidatesService::getCandidates(string idUser){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("SearchCandidatesService"));

	list<UserProfile*> candidates = this->runSearchAlgorithm(idUser, 3000);

    return candidates;
}

list<UserProfile*> SearchCandidatesService::runSearchAlgorithm(string idUser, double maxDistance){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("SearchCandidatesService"));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Ejecutando algoritmo de busqueda de candidatos"));

	list<UserProfile*> users;
	list<UserProfile*> candidates;
	try{
		users = this->sharedService->getUsers();
		UserProfile* userProfile = this->sharedService->getUser(idUser);
		list<Interest*> interests = userProfile->getInterests();

		for (list<UserProfile*>::iterator it=users.begin(); it!=users.end(); ++it){

			UserProfile* user = *it;
			list<Interest*> userInterests = user->getInterests();
			string idCandidate = user->getId();

			//filtro mismo usuario
			if (idUser.compare(idCandidate)==0){
				LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("El candidato "<<idCandidate<<" es filtrado por ser el mismo usuario"));
				continue;
			}

			//filtro de match
			bool eval = matchService->isACandidate(idUser, idCandidate);
			if (!eval){
				LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("El candidato "<<idCandidate<<" es filtrado por ya estar evaluado por los usuarios"));
				continue;
			}

			//filtro de distancia
			double distance = calculateDistance(user->getLocation(), userProfile->getLocation());

			if (distance > maxDistance){
				LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("El candidato "<<idCandidate<<" es filtrado por la distancia"));
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
				LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("El candidato "<<idCandidate<<" se agrega a los resultados de la busqueda"));
				candidates.push_back(user);
				it = users.erase(it);
			}else{
				LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("El candidato "<<idCandidate<<" es filtrado por no tener interes en comun"));
			}
		}

		//Guardo la lista


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


