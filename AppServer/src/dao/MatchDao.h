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

using namespace std;

/**
 * Clase para realizar las operaciones en la base con respecto a la entity
 * Match.
 */
class MatchDao : public GenericDao{
public:
	MatchDao(){};
	virtual ~MatchDao(){};
	Entity* get(string id) const throw(EntityNotFoundException);
	void put(string id,Entity* entity) const throw(InvalidEntityException);
};

#endif /* MATCHDAO_H_ */
