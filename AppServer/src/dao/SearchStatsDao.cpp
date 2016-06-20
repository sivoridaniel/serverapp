/*
 * SearchStatsDao.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: agustin
 */

#include "SearchStatsDao.h"

Entity* SearchStatsDao::get(string id)
{

	string json;

	json = DbHelper::get(id, SEARCH_STATS);

	SearchStats* searchStats = new SearchStats(json);

	return searchStats;
}

void SearchStatsDao::put(string id, Entity* e)
{

	SearchStats* searchStats = dynamic_cast<SearchStats*>(e);
	if (searchStats == 0)
	{
		throw InvalidEntityException();
	}
	std::string json = searchStats->toJson();

	DbHelper::put(id, json, SEARCH_STATS);

}
