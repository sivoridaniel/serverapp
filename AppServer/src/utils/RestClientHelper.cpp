/*
 * RestClientHelper.cpp

 *
 *  Created on: Jun 12, 2016
 *      Author: agustin
 */

#include "RestClientHelper.h"

RestResponse* RestClientHelper::put(string url, string json)
{
	RestClient::response r = RestClient::put(url, "application/json", json);
	RestResponse* response = new RestResponse(r.code, r.body);
	return response;
}

RestResponse* RestClientHelper::del(string url)
{
	RestClient::response r = RestClient::del(url);
	RestResponse* response = new RestResponse(r.code, r.body);
	return response;
}

RestResponse* RestClientHelper::post(string url, string json)
{
	RestClient::response r = RestClient::post(url, "application/json", json);
	RestResponse* response = new RestResponse(r.code, r.body);
	return response;
}

RestResponse* RestClientHelper::get(string url)
{
	RestClient::response r = RestClient::get(url);
	RestResponse* response = new RestResponse(r.code, r.body);
	return response;
}

