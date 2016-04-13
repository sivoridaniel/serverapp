/*
 * UserLogin.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */
#include <string>

#ifndef SRC_MODEL_USERLOGIN_H_
#define SRC_MODEL_USERLOGIN_H_

using namespace std;

class UserLogin {
public:
	UserLogin(int idUser, string username, string password);
	int getIdUser();
	string getUserName();
	string getPassword();
	virtual ~UserLogin();
private:
	int idUser;
	string username;
	string password;
};

#endif /* SRC_MODEL_USERLOGIN_H_ */
