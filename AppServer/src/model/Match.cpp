/*
 * Match.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#include "Match.h"

Match::Match(std::string idUser) {
	this->idUser = idUser;

}

Match::~Match() {
	// TODO Auto-generated destructor stub
}

void Match::acceptUser(std::string idUser){
	this->yesList.push_back(idUser);
}

void Match::rejectUser(std::string idUser){
	this->noList.push_back(idUser);
}

bool Match::isAccepted(std::string idUser){
	bool found = (std::find(yesList.begin(), yesList.end(), idUser)!=yesList.end());
	return found;
}

bool Match::isRejected(std::string idUser){
	bool found = (std::find(noList.begin(), noList.end(), idUser)!=noList.end());
	return found;
}

std::string Match::toJson(){
	Json::Value root;
	Json::FastWriter writer;
	Json::Value vecYes(Json::arrayValue);
	Json::Value vecNo(Json::arrayValue);
	for (std::list<std::string>::iterator it=yesList.begin(); it!=yesList.end(); ++it){
		std::string idUser = *it;
		vecYes.append(idUser);
	}
	for (std::list<std::string>::iterator it=noList.begin(); it!=noList.end(); ++it){
		std::string idUser = *it;
		vecNo.append(idUser);
	}
	root["idUser"] = this->idUser;
	root["acceptedUsers"] = vecYes;
	root["rejectedUsers"] = vecNo;
	std::string json = writer.write(root);
	return json;
}
