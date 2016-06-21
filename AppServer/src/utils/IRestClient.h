#ifndef IRESTCLIENT_H_
#define IRESTCLIENT_H_

#include <restclient-cpp/restclient.h>
#include <restclient-cpp/meta.h>

using namespace std;

class RestResponse
{
public:

	int code;
	string body;

	RestResponse(int code, string body)
	{
		this->code = code;
		this->body = body;
	}
};

class IRestClient
{

public:

	virtual ~IRestClient()
	{
	}
	;

	virtual RestResponse* put(string url, string json)=0;

	virtual RestResponse* del(string url)=0;

	virtual RestResponse* post(string url, string json)=0;

	virtual RestResponse* get(string url)=0;

};

#endif
