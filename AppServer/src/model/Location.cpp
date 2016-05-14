/*
 * Location.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#include "Location.h"

Location::Location(string longitude, string latitude) {
	this->longitude = longitude;
	this->latitude = latitude;

}

Location::~Location() {
	// TODO Auto-generated destructor stub
}

std::string Location::toJson(){
	Json::Value root;
	Json::FastWriter writer;

	root["latitude"] = latitude;
	root["longitude"] = longitude;
	std::string json = writer.write(root);
	return json;
}


