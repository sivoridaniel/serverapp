/*
 * CandidatesDao.cpp
 *
 *  Created on: May 18, 2016
 *      Author: agustin
 */

#include "CandidatesDao.h"

Entity* CandidatesDao::get(string id)
{

	string json;

	json = DbHelper::get(id, CANDIDATE);

	CandidateList* candidates = new CandidateList(json);

	return candidates;
}

void CandidatesDao::put(string id, Entity* e)
{

	CandidateList* candidates = dynamic_cast<CandidateList*>(e);
	if (candidates == 0)
	{
		throw InvalidEntityException();
	}

	string json = candidates->toJson();

	DbHelper::put(id, json, CANDIDATE);

}
