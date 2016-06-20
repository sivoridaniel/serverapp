/*
 * SearchStats.h
 *
 *  Created on: Jun 17, 2016
 *      Author: agustin
 */

#ifndef SEARCHSTATS_H_
#define SEARCHSTATS_H_

#include "Entity.h"
#include <ctime>
#include <list>

using namespace std;

class UserStat
{
public:
	string idUser;
	int likesCount;
	int todaySearchsCount;
	string lastSearch;

	UserStat(string idUser, int likesCount, int todaySearchsCount, string lastSearch)
	{
		this->idUser = idUser;
		this->likesCount = likesCount;
		this->todaySearchsCount = todaySearchsCount;
		this->lastSearch = lastSearch;
	}

	virtual ~UserStat()
	{
	}
	;
};

class SearchStats: public Entity
{
private:
	list<UserStat*> usersStats;
public:
	SearchStats();
	SearchStats(string json);

	virtual ~SearchStats();

	string toJson();

	UserStat* getUserStat(string idUser);

	void addUserStat(UserStat* userStat);

	void addLike(string idUser);

	void updateLastSearch(string idUser);

	list<UserStat*> getUsersStats()
	{
		return this->usersStats;
	}

	list<string> getMostLikedUsers();
};

#endif /* SEARCHSTATS_H_ */
