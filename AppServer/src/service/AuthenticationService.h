/*
 * AuthenticationService.h
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#ifndef AUTHENTICATIONSERVICE_H_
#define AUTHENTICATIONSERVICE_H_

#include "../dao/UserDao.h"
#include <string>
using namespace std;

class AuthenticationService {
private:
	UserDao* userDao;
public:
	AuthenticationService();
	virtual ~AuthenticationService();

};

#endif /* AUTHENTICATIONSERVICE_H_ */
