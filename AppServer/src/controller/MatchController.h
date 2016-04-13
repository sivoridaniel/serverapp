/*
 * MatchController.h
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#ifndef MATCHCONTROLLER_H_
#define MATCHCONTROLLER_H_

#include "../service/MatchService.h"
#include "AbstractController.h"
#include "../model/NewMatchesResponse.h"
#include "../webserver/mongoose.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

class MatchController : public AbstractController{

private:
	MatchService* matchService;
public:
	MatchController();
	virtual ~MatchController();

	string connect(struct mg_connection *nc, struct http_message *hm);

private:

	string event_handler_submit_yes(struct mg_connection *nc, struct http_message *hm);
	string event_handler_submit_no(struct mg_connection *nc, struct http_message *hm);
	string event_handler_new_matches(struct mg_connection *nc, struct http_message *hm);
	string event_handler_confirm_match(struct mg_connection *nc, struct http_message *hm);

};

#endif /* MATCHCONTROLLER_H_ */
