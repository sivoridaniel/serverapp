/*
 * AuthenticationService.h
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#ifndef AUTHENTICATIONSERVICE_H_
#define AUTHENTICATIONSERVICE_H_

#include "../dao/UserDao.h"

using namespace std;

class AuthenticationService {
private:
	UserDao* userDao;
public:
	AuthenticationService(UserDao* userDao);
	virtual ~AuthenticationService();

	string createNewUser(string name,string password,string email);
};

#endif /* AUTHENTICATIONSERVICE_H_ */
