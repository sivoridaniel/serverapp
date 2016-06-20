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

using namespace std;

class Location: public Entity
{
private:
	string longitude;
	string latitude;
public:
	Location(string longitude, string latitude);
	virtual ~Location();

	std::string toJson();

	string getLatitude() const
	{
		return latitude;
	}

	void setLatitude(string latitude)
	{
		this->latitude = latitude;
	}

	string getLongitude() const
	{
		return longitude;
	}

	void setLongitude(string longitude)
	{
		this->longitude = longitude;
	}
};

#endif /* LOCATION_H_ */
