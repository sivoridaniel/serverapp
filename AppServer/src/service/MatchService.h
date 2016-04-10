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

	bool addToYesList(string idUser, string idUserAccepted) throw(IllegalStateException);
	void addToNoList(string idUser, string idUserRejected) throw(IllegalStateException);
	list<string> getNewMatches(string idUser);
	void confirmUser(string idUser, string idUserConfirmed) throw(IllegalStateException);
};

#endif /* MATCHSERVICE_H_ */
