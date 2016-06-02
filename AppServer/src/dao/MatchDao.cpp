/*
 * MatchDao.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#include "MatchDao.h"

Entity* MatchDao::get(string id){

	std::string json;

	json = DbHelper::get(id, MATCH);

	Match* match = new Match(json);

	return match;
}

void MatchDao::put(string id, Entity* e){

	Match* match = dynamic_cast<Match*>(e);
	if (match==0){
		throw InvalidEntityException();
	}
	std::string json = match->toJson();

	DbHelper::put(id,json,MATCH);

}
