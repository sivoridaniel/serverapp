/*
 * SearchStatsDao.h
 *
 *  Created on: Jun 17, 2016
 *      Author: agustin
 */

#ifndef SEARCHSTATSDAO_H_
#define SEARCHSTATSDAO_H_

#include "../model/SearchStats.h"
#include "GenericDao.h"

class SearchStatsDao : public GenericDao{
public:
	SearchStatsDao(){};
	virtual ~SearchStatsDao(){};
	/**
	 * Se obtiene el SearchStat con id pasado por parámetro.
	 * @param id
	 * @throw EntityNotFoundException
	 * @return Entity*
	 */
	Entity* get(string id);
	/**
	 * Se inserta el SearchStatsDao con el id que le corresponde.
	 *
	 * @param id
	 * @param entity
	 * @throw InvalidEntityException
	 */
	void put(string id,Entity* entity);
};

#endif /* SEARCHSTATSDAO_H_ */
