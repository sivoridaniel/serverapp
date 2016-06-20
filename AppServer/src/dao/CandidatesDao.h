/*
 * CandidatesDao.h
 *
 *  Created on: May 18, 2016
 *      Author: agustin
 */

#ifndef CANDIDATESDAO_H_
#define CANDIDATESDAO_H_

#include "../model/CandidateList.h"
#include "GenericDao.h"

using namespace std;

/**
 * Clase para guardar las listas de candidatos por usuario en la base
 */
class CandidatesDao: public GenericDao
{
public:
	CandidatesDao()
	{
	}
	;
	virtual ~CandidatesDao()
	{
	}
	;
	/**
	 * Se obtiene el candidato con id pasado por parámetro.
	 * @param id
	 * @throw EntityNotFoundException
	 * @return Entity*
	 */
	Entity* get(string id);
	/**
	 * Se inserta el candidato con el id que le corresponde.
	 *
	 * @param id
	 * @param entity
	 * @throw InvalidEntityException
	 */
	void put(string id, Entity* entity);
};

#endif /* CANDIDATESDAO_H_ */
