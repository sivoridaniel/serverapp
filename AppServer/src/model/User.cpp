/*
 * User.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "User.h"

User::User(std::string name, std::string password){
	this->name = name;
	this->password = password;
}

User::User(std::string json) {
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
	    throw JsonParseException();
	}
	this->name = root.get("name", "").asString();
	this->password = root.get("password", "").asString();
	this->firstName = root.get("firstName", "").asString();
	this->lastName = root.get("lastName", "").asString();
	this->email = root.get("email", "").asString();
}

User::~User() {
	this->name = "";
	this->password = "";
}

std::string User::toJson(){
	Json::Value root;
	Json::FastWriter writer;
	root["name"] = this->name;
	root["password"] = this->password;
	root["firstName"] = this->firstName;
	root["lastName"] = this->lastName;
	root["email"] = this->email;
	return writer.write(root);
}
