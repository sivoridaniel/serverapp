/*
 * AuthenticationService.h
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#ifndef IAUTHENTICATIONSERVICE_H_
#define IAUTHENTICATIONSERVICE_H_

#include "../exception/IllegalStateException.h"
#include "../exception/IncorrectPasswordException.h"
#include <log4cplus/logger.h>
#include "../model/UserProfile.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

#include "../dao/UserDao.h"

using namespace std;
using namespace log4cplus;

class AuthenticationService {
private:
	IUserDao* userDao;

public:
	AuthenticationService();
	UserProfile* getUserLogin(string username,string password) throw(IncorrectPasswordException, EntityNotFoundException);
	virtual ~AuthenticationService();

};

#endif /* AUTHENTICATIONSERVICE_H_ */
