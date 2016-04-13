/*
 * NewMatchesResponse.h
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#ifndef NEWMATCHESRESPONSE_H_
#define NEWMATCHESRESPONSE_H_

#include "Entity.h"
#include <list>

using namespace std;

class NewMatchesResponse : public Entity{
private:
	list<string> newMatches;
public:
	NewMatchesResponse(list<string> newMatches);
	virtual ~NewMatchesResponse();

	string toJson();

};

#endif /* NEWMATCHESRESPONSE_H_ */
