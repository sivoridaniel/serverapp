/*
 * AbmUserService.h
 *
 *  Created on: 13 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_SERVICE_ABMUSERSERVICE_H_
#define SRC_SERVICE_ABMUSERSERVICE_H_

#include "../dao/UserDao.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

class AbmUserService {
private:
	IUserDao* userDao;
public:
	AbmUserService();
	string createNewUser(string name,string password,string token,string email);
	void modifyUser(UserProfile* userProfile)throw (MergeDbException);
	virtual ~AbmUserService();
};

#endif /* SRC_SERVICE_ABMUSERSERVICE_H_ */
