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
	/**
	 * Se obtiene el Match con id pasado por parámetro.
	 * @param id
	 * @throw EntityNotFoundException
	 * @return Entity*
	 */
	Entity* get(string id);
	/**
	 * Se inserta el match con el id que le corresponde.
	 *
	 * @param id
	 * @param entity
	 * @throw InvalidEntityException
	 */
	void put(string id,Entity* entity);
};

#endif /* MATCHDAO_H_ */
