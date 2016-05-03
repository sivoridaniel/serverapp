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

/**
 * Clase que se utiliza para manejar la authenticación de usuario en caso de
 * que el controller invocado no sea público.
 */
class AuthenticationService {
private:
	UserDao* userDao;

public:
	/**
	 * Crea el UserDao
	 */
	AuthenticationService();
	/**
	 * Constructor que se utiliza para poder ser utilizado con gmock.
	 * De esta manera podemos mockear el DAO para no realizar el llamado a
	 * la base de datos y poder realizar los test unit de los métodos de la
	 * clase.
	 *
	 * @param IUserDao
	 */
	AuthenticationService(UserDao * userDao);
	/**
	 * En caso de poder loguearse, devuelve el perfil del usuario.
	 * En caso de que se pase por parametro password en vacio (""), devolverá
	 * el perfil del usuario correspondiente a username.
	 * Caso contrario disparará la excepcion IncorrectPasswordException.
	 *
	 * @param string username
	 * @param string password
	 * @throw IncorrectPasswordException
	 * @return UserProfile
	 */
	UserProfile* getUserLogin(string username,string password) throw(IncorrectPasswordException, EntityNotFoundException);
	/**
	 * Libera la memoria reservada para IUserDao
	 */
	virtual ~AuthenticationService();

};

#endif /* AUTHENTICATIONSERVICE_H_ */
