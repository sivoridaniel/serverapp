/*
 * SearchCandidatesService.h
 *
 *  Created on: May 7, 2016
 *      Author: agustin
 */

#ifndef SEARCHCANDIDATESSERVICE_H_
#define SEARCHCANDIDATESSERVICE_H_

#include "MatchService.h"
#include "../../exception/IllegalStateException.h"
#include "../../model/UserProfile.h"
#include "RemoteSharedService.h"
#include "../api/ISearchCandidatesService.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>
#include <restclient-cpp/restclient.h>
#include <restclient-cpp/meta.h>


using namespace std;
using namespace log4cplus;

class SearchCandidatesService : public ISearchCandidatesService{
private:
	MatchService* matchService;
	IRemote* sharedService;

public:
	SearchCandidatesService();

	/**
	 * Constructor que se utiliza para poder ser utilizado con gmock.
	 * De esta manera podemos mockear el DAO para no realizar el llamado a
	 * la base de datos y poder realizar los test unit de los métodos de la
	 * clase.
	 *
	 * @param MatchService
	 * @param SharedService
	 */
	SearchCandidatesService( MatchService* matchService, IRemote* sharedService);

	/**
	 * Obtiene una lista de candidatos para el usuario dado que tengan intereses
	 * comunes
	 *
	 * @param idUser
	 */
	list<UserProfile*> getCandidates(string idUser);

	virtual ~SearchCandidatesService();

private:
	list<UserProfile*> runSearchAlgorithm(string idUser, double maxDistance);
	double calculateDistance(Location* location1, Location* location2);

};

#endif /* SEARCHCANDIDATESSERVICE_H_ */
