/*
 * AbmUserService.h
 *
 *  Created on: 13 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_SERVICE_ABMUSERSERVICE_H_
#define SRC_SERVICE_ABMUSERSERVICE_H_

#include "../dao/UserDao.h"
#include <string>
using namespace std;

class AbmUserService {
private:
	UserDao* userDao;
public:
	AbmUserService();
	string createNewUser(string name,string password,string email);
	virtual ~AbmUserService();
};

#endif /* SRC_SERVICE_ABMUSERSERVICE_H_ */
