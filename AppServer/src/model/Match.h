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
	std::string idUser;
	std::list<std::string> yesList;
	std::list<std::string> noList;
public:
	Match(std::string idUser);
	virtual ~Match();

	std::string toJson();

	void acceptUser(std::string idUser);

	void rejectUser(std::string idUser);

	const std::string& getIdUser() const {
		return idUser;
	}

	void setIdUser(const std::string& idUser) {
		this->idUser = idUser;
	}

	bool isAccepted(std::string idUser);

	bool isRejected(std::string idUser);
};

#endif /* MATCH_H_ */
