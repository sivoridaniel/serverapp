/*
 * CandidateList.h
 *
 *  Created on: May 18, 2016
 *      Author: agustin
 */

#ifndef CANDIDATELIST_H_
#define CANDIDATELIST_H_

#include <string>
#include <list>
#include "Entity.h"
#include "UserProfile.h"
#include "../exception/JsonParseException.h"
#include "../json/json.h"

using namespace std;

class CandidateList : public Entity{
private:
	list<UserProfile*> candidates;
public:
	CandidateList();
	CandidateList(string json);
	virtual ~CandidateList();

	string toJson();

	const list<UserProfile*>& getCandidates() const {
		return candidates;
	}

	void setCandidates(const list<UserProfile*>& candidates) {
		this->candidates = candidates;
	}

};

#endif /* CANDIDATELIST_H_ */
