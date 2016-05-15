/*
 * UriParser.cpp
 *
 *  Created on: May 15, 2016
 *      Author: agustin
 */

#include "UriParser.h"


string UriParser::getId(string queryString){
	string s = "id";
	std::vector<std::string> x = split(queryString, '=');
	if (x.size()!=2){
		return "";
	}
	if (s.compare(x[0])!=0) {
		return "";
	}
	return x[1];
}

std::vector<std::string> &UriParser::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> UriParser::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
