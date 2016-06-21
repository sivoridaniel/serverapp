/*
 * DistanceHelper.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: agustin
 */

#include "DistanceHelper.h"

double DistanceHelper::deg2rad(double deg) {
  return (deg * pi / 180);
}

double DistanceHelper::rad2deg(double rad) {
  return (rad * 180 / pi);
}

double DistanceHelper::distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}
