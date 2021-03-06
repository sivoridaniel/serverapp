/*
 * UriParser.h
 *
 *  Created on: May 15, 2016
 *      Author: agustin
 */

#ifndef URIPARSER_H_
#define URIPARSER_H_

#include <string>
#include <sstream>
#include <vector>

using namespace std;

class UriParser
{
public:
	/**
	 * Obtiene en un vector de string los parametros de consulta de una uri
	 * @param queryString
	 * @return vector<string>
	 */
	static vector<string> getParams(string queryString);

private:

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

	static std::vector<std::string> split(const std::string &s, char delim);
};

#endif /* URIPARSER_H_ */
