/*
 * MatchDao.h
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#ifndef MATCHDAO_H_
#define MATCHDAO_H_

#include "../model/Match.h"
#include "GenericDao.h"

class MatchDao : public GenericDao{
public:
	MatchDao(){};
	virtual ~MatchDao(){};
	Entity* get(std::string id) throw(EntityNotFoundException);
	void put(Entity* entity) throw(InvalidEntityException);
};

#endif /* MATCHDAO_H_ */
