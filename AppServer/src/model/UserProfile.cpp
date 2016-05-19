/*
 * User.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "UserProfile.h"

UserProfile::UserProfile(string name, string password){
	this->name = name;
	this->password = password;
	this->location = new Location(0,0);
}

UserProfile::UserProfile(string json) {
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
	    throw JsonParseException();
	}
	this->id = root["user"].get("id", "").asString();
	this->alias = root["user"].get("alias", "").asString();
	this->name = root["user"].get("name", "").asString();
	this->password = root["user"].get("password", "").asString();
	this->token = root["user"].get("token", "").asString();
	this->email = root["user"].get("email", "").asString();
	this->photoProfile = root["user"].get("photo", "").asString();
	string latitude = root["user"]["location"].get("latitude", "").asString();
	string longitude = root["user"]["location"].get("longitude", "").asString();

	this->location = new Location(latitude,longitude);

	const Json::Value interests = root["user"]["interests"];
	for (unsigned int i = 0; i < interests.size(); ++i){
		string category = interests[i]["category"].asString();
		string value = interests[i]["value"].asString();
		Interest* interest = new Interest(category, value);
		this->addInterest(interest);
	}
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

string UserProfile::toJson(){
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	for (list< Interest* >::iterator it=interests.begin(); it!=interests.end(); ++it){
		Interest* interest = *it;
		string jsonInterest = interest->toJson();
		vecInterests.append(jsonInterest);
	}

	string jsonLocation = this->location->toJson();

	root["user"]["id"] = this->id;
	root["user"]["name"] = this->name;
	root["user"]["alias"] = this->alias;
	root["user"]["token"] = this->token;
	root["user"]["email"] = this->email;
	root["user"]["photo"] = this->photoProfile;
	root["user"]["interests"] = vecInterests;
	root["user"]["location"] = jsonLocation;
	root["user"]["password"] = this->password;
	string json = writer.write(root);
	return json;
}

string UserProfile::toSharedJson(){
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	root["user"]["name"] = this->name;
	root["user"]["alias"] = this->alias;
	root["user"]["photo"] = this->photoProfile;
	root["user"]["location"]["latitude"] = this->location->getLatitude();
	root["user"]["location"]["longitude"] = this->location->getLongitude();
	int i=0;
	for (list< Interest* >::iterator it=interests.begin(); it!=interests.end(); ++it){
		Interest* interest = *it;
		root["user"]["interests"][i]["category"] = interest->getCategory();;
		root["user"]["interests"][i]["value"] = interest->getValue();
		i++;
	}
	root["user"]["email"] = this->email;

	string json = writer.write(root);
	return json;

}

void UserProfile::addInterest(Interest* interest){
	this->interests.push_back(interest);
}
