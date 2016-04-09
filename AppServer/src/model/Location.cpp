/*
 * Location.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#include "Location.h"

Location::Location(double longitude, double latitude) {
	this->longitude = longitude;
	this->latitude = latitude;

}

Location::~Location() {
	// TODO Auto-generated destructor stub
}

std::string Location::toJson(){
	Json::Value root;
	Json::FastWriter writer;
	std::ostringstream sLatitude;
	std::ostringstream sLongitude;
	sLatitude << this->latitude;
	sLongitude << this->longitude;
	root["latitude"] = sLatitude.str();
	root["longitude"] = sLongitude.str();
	std::string json = writer.write(root);
	return json;
}


