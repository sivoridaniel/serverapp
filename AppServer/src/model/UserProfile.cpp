/*
 * User.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "UserProfile.h"

UserProfile::UserProfile(std::string name, std::string password){
	this->name = name;
	this->password = password;
	this->location = new Location(0,0);
}

UserProfile::UserProfile(std::string json) {
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
	    throw JsonParseException();
	}
	this->name = root.get("name", "").asString();
	this->password = root.get("password", "").asString();
	this->token = root.get("token", "").asString();
	this->email = root.get("email", "").asString();
	this->location = new Location(0,0);
}

UserProfile::~UserProfile() {
	this->name = "";
	this->password = "";
	this->token = "";
	for (std::list< Interest* >::iterator it=interests.begin(); it!=interests.end(); ++it){
		Interest* interest = *it;
		delete interest;
	}
	delete location;
}

std::string UserProfile::toJson(){
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	for (std::list< Interest* >::iterator it=interests.begin(); it!=interests.end(); ++it){
		Interest* interest = *it;
		std::string jsonInterest = interest->toJson();
		vecInterests.append(jsonInterest);
	}

	std::string jsonLocation = this->location->toJson();

	root["id"] = this->id;
	root["name"] = this->name;
	root["alias"] = this->alias;
	root["token"] = this->token;
	root["email"] = this->email;
	root["photo_profile"] = this->photoProfile;
	root["interests"] = vecInterests;
	root["location"] = jsonLocation;
	root["password"] = this->password;
	std::string json = writer.write(root);
	return json;
}

void UserProfile::addInterest(Interest* interest){
	this->interests.push_back(interest);
}
