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
class CandidatesDao : public GenericDao{
public:
	CandidatesDao(){};
	virtual ~CandidatesDao(){};
	Entity* get(string id) const throw(EntityNotFoundException);
	void put(string id,Entity* entity) const throw(InvalidEntityException);
};

#endif /* CANDIDATESDAO_H_ */
