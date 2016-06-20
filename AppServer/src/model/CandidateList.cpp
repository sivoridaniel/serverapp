/*
 * CandidateList.cpp
 *
 *  Created on: May 18, 2016
 *      Author: agustin
 */

#include "CandidateList.h"

CandidateList::CandidateList()
{

}

CandidateList::CandidateList(string json)
{
	Json::Value root;
	Json::Reader reader;
	Json::FastWriter writer;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok)
	{
		throw JsonParseException();
	}

	const Json::Value users = root["users"];
	for (unsigned int i = 0; i < users.size(); ++i)
	{
		Json::Value jsonUserValue = users[i];
		string jsonUser = writer.write(jsonUserValue);
		UserProfile* user = new UserProfile(jsonUser);
		candidates.push_back(user);
	}
}

CandidateList::~CandidateList()
{
	for (std::list<UserProfile*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
	{
		UserProfile* candidate = *it;
		delete candidate;
	}
}

string CandidateList::toJson()
{
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	int i = 0;
	for (list<UserProfile*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
	{
		UserProfile* candidate = *it;
		Location* location = candidate->getLocation();
		list<Interest*> interests = candidate->getInterests();

		root["users"][i]["user"]["name"] = candidate->getName();
		root["users"][i]["user"]["alias"] = candidate->getAlias();
		root["users"][i]["user"]["sex"] = candidate->getSex();
		root["users"][i]["user"]["age"] = candidate->getAge();
		root["users"][i]["user"]["photo"] = candidate->getPhotoProfile();
		root["users"][i]["user"]["location"]["latitude"] = location->getLatitude();
		root["users"][i]["user"]["location"]["longitude"] = location->getLongitude();
		int j = 0;
		for (list<Interest*>::iterator itI = interests.begin(); itI != interests.end(); ++itI)
		{
			Interest* interest = *itI;
			root["users"][i]["user"]["interests"][j]["category"] = interest->getCategory();
			;
			root["users"][i]["user"]["interests"][j]["value"] = interest->getValue();
			j++;
		}
		root["users"][i]["user"]["email"] = candidate->getEmail();
		i++;

	}

	string json = writer.write(root);
	return json;
}

