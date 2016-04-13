/*
 * NewMatchesResponse.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#include "NewMatchesResponse.h"

NewMatchesResponse::NewMatchesResponse(list<string> newMatches) {
	this->newMatches = newMatches;

}

NewMatchesResponse::~NewMatchesResponse() {
	// TODO Auto-generated destructor stub
}


string NewMatchesResponse::toJson(){
	Json::Value root;
	Json::FastWriter writer;

	Json::Value vecMatches(Json::arrayValue);

	for (list<string>::iterator it=newMatches.begin(); it!=newMatches.end(); ++it){
		std::string idUser = *it;
		vecMatches.append(idUser);
	}
	root["newmatches"] = vecMatches;
	string json = writer.write(root);
	return json;
}

