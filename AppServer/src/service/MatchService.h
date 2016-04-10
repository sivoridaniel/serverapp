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

using namespace std;

class MatchService {
private:
	MatchDao* matchDao;
	ChatDao* chatDao;
public:
	MatchService();
	virtual ~MatchService();

	string addToYesList(string idUser, string idUserAccepted) throw(IllegalStateException);
	string addToNoList(string idUser, string idUserRejected) throw(IllegalStateException);
	string getNewMatches(string idUser);
	string confirmUser(string idUser, string idUserConfirmed) throw(IllegalStateException);
};

#endif /* MATCHSERVICE_H_ */
