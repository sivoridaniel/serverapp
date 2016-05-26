/*
 * RemoteSharedService.h
 *
 *  Created on: May 14, 2016
 *      Author: agustin
 */

#ifndef REMOTESHAREDSERVICE_H_
#define REMOTESHAREDSERVICE_H_

#include "../model/UserProfile.h"
#include "../model/Interest.h"
#include "../exception/EntityExistsException.h"
#include "../exception/EntityNotFoundException.h"
#include "../exception/RemoteException.h"
#include "IRemote.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <restclient-cpp/restclient.h>
#include <restclient-cpp/meta.h>

using namespace std;
using namespace log4cplus;


class RemoteSharedService : public IRemote{

private:
	string url;

public:
	RemoteSharedService(){
		url = "http://shared-server-match.herokuapp.com";
	}
	RemoteSharedService(string url);
	virtual ~RemoteSharedService();

	void createInterest(Interest* interest);

	list<Interest*> getInterests();

	list<UserProfile*> getUsers();

	UserProfile* getUser(string id);

	void createUser(UserProfile* userProfile);

	void deleteUser(string id);

	void updateUser(UserProfile* userProfile);

private:

	list<Interest*> parseInterests(string json);
	list<UserProfile*> parseUsers(string json);

};

#endif /* REMOTESHAREDSERVICE_H_ */
