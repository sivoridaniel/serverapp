/*
 * Match.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#include "Match.h"

Match::Match(std::string json) {

	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
	    throw JsonParseException();
	}
	this->idUser = root.get("name", "").asString();
	const Json::Value acceptedUsers = root["acceptedUsers"];
	for (unsigned int i = 0; i < acceptedUsers.size(); ++i){
		std::string acceptedUser = acceptedUsers[i].asString();
		this->yesList.push_back(acceptedUser);
	}
	const Json::Value rejectedUsers = root["rejectedUsers"];
	for (unsigned int i = 0; i < rejectedUsers.size(); ++i){
		std::string rejectedUser = rejectedUsers[i].asString();
		this->noList.push_back(rejectedUser);
	}
	const Json::Value matchedUsers = root["matchedUsers"];
	for (unsigned int i = 0; i < matchedUsers.size(); ++i){
		std::string matchedUser = matchedUsers[i].asString();
		this->newMatches.push_back(matchedUser);
	}

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

void Match::addNewMatch(std::string idUser){
	this->newMatches.push_back(idUser);
}


bool Match::isAccepted(std::string idUser){
	bool found = (std::find(yesList.begin(), yesList.end(), idUser)!=yesList.end());
	return found;
}

bool Match::isRejected(std::string idUser){
	bool found = (std::find(noList.begin(), noList.end(), idUser)!=noList.end());
	return found;
}

bool Match::isMatched(std::string idUser){
	bool found = (std::find(newMatches.begin(), newMatches.end(), idUser)!=newMatches.end());
	return found;
}


void Match::removeFromYesList(std::string idUser){
	yesList.remove(idUser);
}

void Match::removeFromNoList(std::string idUser){
	noList.remove(idUser);
}

void Match::removeFromNewMatches(std::string idUser){
	newMatches.remove(idUser);
}

std::string Match::toJson(){
	Json::Value root;
	Json::FastWriter writer;
	Json::Value vecYes(Json::arrayValue);
	Json::Value vecNo(Json::arrayValue);
	Json::Value vecMatches(Json::arrayValue);
	for (std::list<std::string>::iterator it=yesList.begin(); it!=yesList.end(); ++it){
		std::string idUser = *it;
		vecYes.append(idUser);
	}
	for (std::list<std::string>::iterator it=noList.begin(); it!=noList.end(); ++it){
		std::string idUser = *it;
		vecNo.append(idUser);
	}
	for (std::list<std::string>::iterator it=newMatches.begin(); it!=newMatches.end(); ++it){
		std::string idUser = *it;
		vecMatches.append(idUser);
	}
	root["idUser"] = this->idUser;
	root["acceptedUsers"] = vecYes;
	root["rejectedUsers"] = vecNo;
	root["matchedUsers"] = vecMatches;
	std::string json = writer.write(root);
	return json;
}
