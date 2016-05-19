/*
 * Match.h
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#ifndef MATCH_H_
#define MATCH_H_

#include <string>
#include <list>
#include <algorithm>
#include "Entity.h"

class Match : public Entity{
private:
	std::list<std::string> yesList;
	std::list<std::string> noList;
	std::list<std::string> newMatches;
public:

	Match();
	Match(std::string json);
	virtual ~Match();

	std::string toJson();

	void acceptUser(std::string idUser);

	void rejectUser(std::string idUser);

	void addNewMatch(std::string idUser);

	bool isAccepted(std::string idUser);

	bool isRejected(std::string idUser);

	bool isMatched(std::string idUser);

	void removeFromYesList(std::string idUser);

	void removeFromNoList(std::string idUser);

	void removeFromNewMatches(std::string idUser);

	std::list<std::string> getNewMatches(){
		return this->newMatches;
	}
};

#endif /* MATCH_H_ */
