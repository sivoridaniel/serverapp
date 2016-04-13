/*
 * MatchService.h
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#ifndef MATCHSERVICE_H_
#define MATCHSERVICE_H_

#include "../dao/MatchDao.h"
#include "../dao/ChatDao.h"
#include "../exception/IllegalStateException.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

class MatchService {
private:
	MatchDao* matchDao;
	ChatDao* chatDao;
public:
	MatchService();
	virtual ~MatchService();

	bool addToYesList(string idUser, string idUserAccepted) throw(IllegalStateException, EntityNotFoundException);
	void addToNoList(string idUser, string idUserRejected) throw(IllegalStateException, EntityNotFoundException);
	list<string> getNewMatches(string idUser) throw (EntityNotFoundException);
	void confirmUser(string idUser, string idUserConfirmed) throw(IllegalStateException, EntityNotFoundException);
};

#endif /* MATCHSERVICE_H_ */
