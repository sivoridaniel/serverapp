/*
 * SearchCandidatesController.cpp
 *
 *  Created on: May 12, 2016
 *      Author: agustin
 */

#include "SearchCandidatesController.h"

SearchCandidatesController::SearchCandidatesController(string url)
{
	this->url = url;
	searchService = new SearchCandidatesService(url);
}

SearchCandidatesController::SearchCandidatesController(ISearchCandidatesService* searchService)
{
	this->searchService = searchService;
}

SearchCandidatesController::~SearchCandidatesController()
{
	delete searchService;
}

string SearchCandidatesController::connect(struct mg_connection *nc, struct http_message *hm, bool test)
{

	if (mg_vcmp(&hm->uri, "/candidates") == 0)
	{

		if (mg_vcmp(&hm->method, "GET") == 0)
		{
			string token = isLogged(hm, test);
			if (!token.empty())
			{
				return event_handler_search_candidates(nc, hm, token);
			}
			else
			{
				return sendForbiddenResponse(nc);
			}
		}

	}
	return STATUS_NOT_FOUND; //Por default devuelve un JSON vacío.

}

string SearchCandidatesController::event_handler_search_candidates(struct mg_connection *nc, struct http_message *hm, string token)
{
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("SearchCandidatesController"));

	string json = "";
	string code = "";
	string id = "";
	string query = string((&hm->query_string)->p, (&hm->query_string)->len);
	vector<string> params = UriParser::getParams(query);

	if (params.size() != 1)
	{
		code = STATUS_NOK;
		json = this->getGenericJson("false", "invalid params");
	}
	else
	{
		id = params[0];
	}
	if (id.compare("") == 0)
	{
		string code = STATUS_NOK;
		json = this->getGenericJson("false", "invalid user in params");
	}
	else
	{
		/* Call match service */
		LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Se buscan candidatos para el usuario " << id));
		try
		{
			list<UserProfile*> candidates = searchService->getCandidates(id);
			json = createSearchResponse(candidates);
			code = STATUS_OK;
		} catch (EntityNotFoundException& e)
		{
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			json = this->getGenericJson("false", e.what());
			code = STATUS_NOT_FOUND;
		} catch (exception& e)
		{
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			code = STATUS_NOK;
			json = this->getGenericJson("false", e.what());
		}
	}

	this->sendResponse(nc, code, json, token);

	return code;

}

string SearchCandidatesController::createSearchResponse(list<UserProfile*> candidates)
{

	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	if (candidates.size() == 0)
	{
		return "{ \"users\": []}";
	}

	int i = 0;
	for (list<UserProfile*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
	{
		UserProfile* user = *it;
		Location* location = user->getLocation();
		list<Interest*> interests = user->getInterests();

		root["users"][i]["user"]["name"] = user->getName();
		root["users"][i]["user"]["alias"] = user->getAlias();
		root["users"][i]["user"]["photo"] = user->getPhotoProfile();
		root["users"][i]["user"]["sex"] = user->getSex();
		root["users"][i]["user"]["age"] = user->getAge();
		root["users"][i]["user"]["location"]["latitude"] = location->getLatitude();
		root["users"][i]["user"]["location"]["longitude"] = location->getLongitude();
		int j = 0;
		for (list<Interest*>::iterator itI = interests.begin(); itI != interests.end(); ++itI)
		{
			Interest* interest = *itI;
			root["users"][i]["user"]["interests"][j]["category"] = interest->getCategory();
			;
			root["users"][i]["user"]["interests"][j]["value"] = interest->getValue();
			j++;
		}
		root["users"][i]["user"]["email"] = user->getEmail();
		root["users"][i]["user"]["id"] = user->getId();
		i++;
		delete user;
	}

	string json = writer.write(root);
	return json;
}

