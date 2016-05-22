/*
 * UriParser.cpp
 *
 *  Created on: May 15, 2016
 *      Author: agustin
 */

#include "UriParser.h"


vector<string> UriParser::getParams(string queryString){
	std::vector<string> params;
	std::vector<std::string> p = split(queryString, '&');
	for (vector<string>::iterator it = p.begin(); it!=p.end(); ++it ){
		string param = *it;
		std::vector<std::string> x = split(param, '=');
		params.push_back(x[1]);
	}
	return params;
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
