/*
 * SearchStats.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: agustin
 */

#include "SearchStats.h"

SearchStats::SearchStats() {

}

SearchStats::SearchStats(string json) {
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
	    throw JsonParseException();
	}
	const Json::Value stats = root["stats"];
	for (unsigned int i = 0; i < stats.size(); ++i){
		string idUser = stats[i]["idUser"].asString();
		int likesCount = stats[i]["likesCount"].asInt();
		int todaySearchs = stats[i]["todaySearchs"].asInt();
		string lastSearch = stats[i]["lastSearch"].asString();

		UserStat* userStat = new UserStat(idUser, likesCount, todaySearchs, lastSearch);
		this->usersStats.push_back(userStat);
	}
}

SearchStats::~SearchStats() {
	for (list< UserStat* >::iterator it=usersStats.begin(); it!=usersStats.end(); ++it){
		UserStat* userStat = *it;
		delete userStat;
	}
}

string SearchStats::toJson(){
	Json::Value root;
	Json::FastWriter writer;

	int i=0;
	for (list< UserStat* >::iterator it=usersStats.begin(); it!=usersStats.end(); ++it){
		UserStat* userStat = *it;
		root["stats"][i]["idUser"] = userStat->idUser;
		root["stats"][i]["likesCount"] = userStat->likesCount;
		root["stats"][i]["todaySearchs"] = userStat->todaySearchsCount;
		root["stats"][i]["lastSearch"] = userStat->lastSearch;
		i++;
	}

	string json = writer.write(root);
	return json;
}

void SearchStats::addUserStat(UserStat* userStat){
	usersStats.push_back(userStat);
}

UserStat* SearchStats::getUserStat(string idUser){
	for (list< UserStat* >::iterator it=usersStats.begin(); it!=usersStats.end(); ++it){
		UserStat* userStat = *it;
		if (userStat->idUser == idUser){
			return userStat;
		}
	}
	return NULL;
}


void SearchStats::addLike(string idUser){
	for (list< UserStat* >::iterator it=usersStats.begin(); it!=usersStats.end(); ++it){
		UserStat* userStat = *it;
		if (userStat->idUser == idUser){
			userStat->likesCount++;
			break;
		}
	}
}

void SearchStats::updateLastSearch(string idUser){
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer [15];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer,15,"%Y-%m-%d",timeinfo);

	string lastSearch(buffer);
	for (list< UserStat* >::iterator it=usersStats.begin(); it!=usersStats.end(); ++it){
		UserStat* userStat = *it;
		if (userStat->idUser == idUser){
			if (userStat->lastSearch != lastSearch){
				userStat->todaySearchsCount = 1;
			}else{
				userStat->todaySearchsCount++;
			}
			userStat->lastSearch = lastSearch;
			break;
		}
	}
}

list<string> SearchStats::getMostLikedUsers(){
	int userCount = usersStats.size();
	int onePercent = (int)(userCount*0.01);

	usersStats.sort([](UserStat* user1, UserStat* user2){ return user1->likesCount > user2->likesCount ;});
	int i=0;
	list<string> mostLikedUsers;
	for (list< UserStat* >::iterator it=usersStats.begin(); it!=usersStats.end(); ++it){
		UserStat* userStat = *it;
		mostLikedUsers.push_back(userStat->idUser);
		if (i >= onePercent){
			break;
		}
		i++;
	}
	return mostLikedUsers;
}

