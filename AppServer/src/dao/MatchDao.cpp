/*
 * MatchDao.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#include "MatchDao.h"

Entity* MatchDao::get(std::string id) throw(EntityNotFoundException){

	std::string json;

	rocksdb::Status s = this->db->Get(rocksdb::ReadOptions(),id,&json);
	if (!s.ok())
	{
		throw EntityNotFoundException();
	}

	Match* match = new Match(json);

	return match;
}

void MatchDao::put(Entity* e) throw(InvalidEntityException){

	Match* match = dynamic_cast<Match*>(e);
	if (match==0){
		throw InvalidEntityException();
	}
	std::string id = match->getIdUser();
	std::string json = match->toJson();
	rocksdb::Status s = this->db->Put(rocksdb::WriteOptions(),id,json);
	if (!s.ok())
	{
		throw std::exception();
	}

}
