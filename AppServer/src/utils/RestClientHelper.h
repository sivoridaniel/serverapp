/*
 * RestClientHelper.h
 *
 *  Created on: Jun 12, 2016
 *      Author: agustin
 */

#ifndef RESTCLIENTHELPER_H_
#define RESTCLIENTHELPER_H_

#include "IRestClient.h"

using namespace std;

class RestClientHelper: public IRestClient
{
public:

	RestResponse* put(string url, string json);

	RestResponse* del(string url);

	RestResponse* post(string url, string json);

	RestResponse* get(string url);

};

#endif /* RESTCLIENTHELPER_H_ */
