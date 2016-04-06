/*
 * AuthenticationService.h
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#ifndef AUTHENTICATIONSERVICE_H_
#define AUTHENTICATIONSERVICE_H_

#include "../dao/UserDao.h"

class AuthenticationService {
private:
	UserDao* userDao;
public:
	AuthenticationService(UserDao* userDao);
	virtual ~AuthenticationService();

	std::string createNewUser(std::string name,
			 std::string password,
			 std::string firstName,
			 std::string lastName,
			 std::string email);
};

#endif /* AUTHENTICATIONSERVICE_H_ */
