/*
 * DistanceHelper.h
 *
 *  Created on: Jun 20, 2016
 *      Author: agustin
 */

#ifndef DISTANCEHELPER_H_
#define DISTANCEHELPER_H_

#include <cmath>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0


class DistanceHelper
{

public:

	// This function converts decimal degrees to radians
	static double deg2rad(double deg);

	//  This function converts radians to decimal degrees
	static double rad2deg(double rad);

	/**
	 * Returns the distance between two points on the Earth.
	 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
	 * @param lat1d Latitude of the first point in degrees
	 * @param lon1d Longitude of the first point in degrees
	 * @param lat2d Latitude of the second point in degrees
	 * @param lon2d Longitude of the second point in degrees
	 * @return The distance between the two points in kilometers
	 */
	static double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);

};

#endif /* DISTANCEHELPER_H_ */
