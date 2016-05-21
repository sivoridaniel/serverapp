/*
 * RemoteSharedService.cpp
 *
 *  Created on: May 14, 2016
 *      Author: agustin
 */

#include "RemoteSharedService.h"

RemoteSharedService::RemoteSharedService(string url) {
	//http://shared-server-match.herokuapp.com
	this->url = url;

}

RemoteSharedService::~RemoteSharedService() {
	// TODO Auto-generated destructor stub
}

void RemoteSharedService::createInterest(Interest* interest) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	string json = interest->toJson();

	RestClient::response r = RestClient::post(url + "/interests",
			"application/json", json);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response body "<<r.body));
	if (r.code == 400) {
		LOG4CPLUS_WARN(logger,
				LOG4CPLUS_TEXT("El interes ya se encuentra registrado en el shared"));
		throw EntityExistsException();
	}

	if (r.code != 200) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El shared server retorno error"));
		throw RemoteException();
	}
}

list<Interest*> RemoteSharedService::getInterests() {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestClient::response r = RestClient::get(url + "/interests");
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response body "<<r.body));

	if (r.code != 200) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El shared server retorno error"));
		throw RemoteException();
	}

	string json = r.body;
	list<Interest*> listInterests = parseInterests(json);
	return listInterests;
}

list<UserProfile*> RemoteSharedService::getUsers() {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestClient::response r = RestClient::get(url + "/users");
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response body "<<r.body));

	if (r.code != 200) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El shared server retorno error"));
		throw RemoteException();
	}

	string json = r.body;
	list<UserProfile*> listUsers = parseUsers(json);
	return listUsers;
}

UserProfile* RemoteSharedService::getUser(string id) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestClient::response r = RestClient::get(url + "/users/" + id);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response body "<<r.body));

	if (r.code == 404) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario no se encuentra"));
		throw EntityNotFoundException();
	}

	if (r.code != 200) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El shared server retorno error"));
		throw RemoteException();
	}

	string json = r.body;

	UserProfile* user = new UserProfile(json);
	return user;
}

void RemoteSharedService::createUser(UserProfile* userProfile) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	string id = userProfile->getId();
	string json = userProfile->toSharedJson();

	RestClient::response r = RestClient::post(url + "/users/" + id,
			"application/json", json);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response body "<<r.body));
	if (r.code == 400) {
		LOG4CPLUS_WARN(logger,
				LOG4CPLUS_TEXT("El usuario ya se encuentra registrado en el shared"));
		throw EntityExistsException();
	}

	if (r.code != 200) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El shared server retorno error"));
		throw RemoteException();
	}
}

void RemoteSharedService::updateUser(UserProfile* userProfile) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	string id = userProfile->getId();
	string json = userProfile->toSharedJson();

	RestClient::response r = RestClient::put(url + "/users/" + id,
			"application/json", json);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response body "<<r.body));
	if (r.code == 404) {
		LOG4CPLUS_WARN(logger,
				LOG4CPLUS_TEXT("El usuario o interes no se encuentra registrado en el shared"));
		throw EntityNotFoundException();
	}

	if (r.code != 200) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El shared server retorno error"));
		throw RemoteException();
	}
}

void RemoteSharedService::deleteUser(string id) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RemoteSharedService"));

	RestClient::response r = RestClient::del(url + "/users/" + id);
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response code "<<r.code));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Response body "<<r.body));

	if (r.code == 404) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario no se encuentra"));
		throw EntityNotFoundException();
	}

	if (r.code != 200) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El shared server retorno error"));
		throw RemoteException();
	}
}

list<Interest*> RemoteSharedService::parseInterests(string json) {
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
		throw JsonParseException();
	}
	list<Interest*> listInterests;
	const Json::Value interests = root["interests"];
	for (unsigned int i = 0; i < interests.size(); ++i) {
		std::string category = interests[i]["category"].asString();
		std::string value = interests[i]["value"].asString();
		Interest* interest = new Interest(category, value);
		listInterests.push_back(interest);
	}
	return listInterests;
}

list<UserProfile*> RemoteSharedService::parseUsers(string json) {
	Json::Value root;
	Json::Reader reader;
	Json::FastWriter writer;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok) {
		throw JsonParseException();
	}

	const Json::Value users = root["users"];
	list<UserProfile*> listUsers;
	for (unsigned int i = 0; i < users.size(); ++i) {
		Json::Value jsonUserValue = users[i];
		string jsonUser =  writer.write(jsonUserValue);
		UserProfile* user = new UserProfile(jsonUser);
		listUsers.push_back(user);
	}

	return listUsers;
}
