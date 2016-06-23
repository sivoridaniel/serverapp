/*
 * RemoteSharedService.cpp
 *
 *  Created on: May 14, 2016
 *      Author: agustin
 */

#include "RemoteSharedService.h"

RemoteSharedService::RemoteSharedService()
{
	this->url = "http://shared-server-match.herokuapp.com";
	this->restClient = new RestClientHelper();
}

RemoteSharedService::RemoteSharedService(string url)
{
	//http://shared-server-match.herokuapp.com
	this->url = url;
	this->restClient = new RestClientHelper();
}

RemoteSharedService::RemoteSharedService(string url, IRestClient* restClient)
{
	this->url = url;
	this->restClient = restClient;
}

RemoteSharedService::~RemoteSharedService()
{
	delete restClient;
}

void RemoteSharedService::createInterest(Interest* interest)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	string json = interest->toJson();

	RestResponse* r = restClient->post(url + "/interests", json);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));
	if (r->code == 400)
	{
		LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("El interes ya se encuentra registrado en el shared"));
		delete r;
		throw EntityExistsException();
	}

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}
	delete r;
}

list<Interest*> RemoteSharedService::getInterests()
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestResponse* r = restClient->get(url + "/interests");
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}

	string json = r->body;
	list<Interest*> listInterests = parseInterests(json);
	delete r;
	return listInterests;
}

list<UserProfile*> RemoteSharedService::getUsers()
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestResponse* r = restClient->get(url + "/users");
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}

	string json = r->body;
	list<UserProfile*> listUsers = parseUsers(json);
	delete r;
	return listUsers;
}

UserProfile* RemoteSharedService::getUser(string id)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestResponse* r = restClient->get(url + "/users/" + id);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));

	if (r->code == 404)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario no se encuentra"));
		delete r;
		throw EntityNotFoundException();
	}

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}

	string json = r->body;

	UserProfile* user = new UserProfile(json);
	delete r;
	return user;
}

string RemoteSharedService::getPhoto(string id)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestResponse* r = restClient->get(url + "/users/" + id + "/photo");
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));

	if (r->code == 404)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario no se encuentra"));
		delete r;
		throw EntityNotFoundException();
	}

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}

	string json = r->body;
	string photo = parsePhoto(json);
	delete r;
	return photo;
}

void RemoteSharedService::createUser(UserProfile* userProfile)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	string id = userProfile->getId();
	string json = userProfile->toSharedJson();

	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("JSON CREATE USER: "<<json));

	RestResponse* r = restClient->post(url + "/users/" + id, json);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));
	if (r->code == 400)
	{
		LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("El usuario ya se encuentra registrado en el shared"));
		delete r;
		throw EntityExistsException();
	}

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}

	//Devolvio 200, seteo el id
	UserProfile* userAux = new UserProfile(r->body);
	userProfile->setId(userAux->getId());
	delete r;
	delete userAux;
}

void RemoteSharedService::updateUser(UserProfile* userProfile)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	string id = userProfile->getId();
	string json = userProfile->toSharedJson();

	RestResponse* r = restClient->put(url + "/users/" + id, json);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));
	if (r->code == 404)
	{
		LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("El usuario o interes no se encuentra registrado en el shared"));
		delete r;
		throw EntityNotFoundException();
	}

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}
	delete r;
}

void RemoteSharedService::deleteUser(string id)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestResponse* r = restClient->del(url + "/users/" + id);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r->code));
	LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Response body "<<r->body));

	if (r->code == 404)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario no se encuentra"));
		delete r;
		throw EntityNotFoundException();
	}

	if (r->code != 200)
	{
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El shared server retorno error"));
		delete r;
		throw RemoteException();
	}
	delete r;
}

list<Interest*> RemoteSharedService::parseInterests(string json)
{
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok)
	{
		throw JsonParseException();
	}
	list<Interest*> listInterests;
	const Json::Value interests = root["interests"];
	for (unsigned int i = 0; i < interests.size(); ++i)
	{
		std::string category = interests[i]["category"].asString();
		std::string value = interests[i]["value"].asString();
		Interest* interest = new Interest(category, value);
		listInterests.push_back(interest);
	}
	return listInterests;
}

list<UserProfile*> RemoteSharedService::parseUsers(string json)
{
	Json::Value root;
	Json::Reader reader;
	Json::FastWriter writer;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok)
	{
		throw JsonParseException();
	}

	const Json::Value users = root["users"];
	list<UserProfile*> listUsers;
	for (unsigned int i = 0; i < users.size(); ++i)
	{
		Json::Value jsonUserValue = users[i];
		string jsonUser = writer.write(jsonUserValue);
		UserProfile* user = new UserProfile(jsonUser);
		listUsers.push_back(user);
	}

	return listUsers;
}

string RemoteSharedService::parsePhoto(string json)
{
	Json::Value root;
	Json::Reader reader;
	Json::FastWriter writer;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok)
	{
		throw JsonParseException();
	}

	string photo = root["photo"].asString();

	return photo;
}
