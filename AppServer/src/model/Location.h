/*
 * Location.h
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#ifndef LOCATION_H_
#define LOCATION_H_

#include <string>
#include "Entity.h"

class Location : public Entity{
private:
	double longitude;
	double latitude;
public:
	Location(double longitude, double latitude);
	virtual ~Location();

	std::string toJson();

	double getLatitude() const {
		return latitude;
	}

	void setLatitude(double latitude) {
		this->latitude = latitude;
	}

	double getLongitude() const {
		return longitude;
	}

	void setLongitude(double longitude) {
		this->longitude = longitude;
	}
};

#endif /* LOCATION_H_ */
