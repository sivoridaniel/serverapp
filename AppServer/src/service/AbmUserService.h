/*
 * AbmUserService.h
 *
 *  Created on: 13 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_SERVICE_ABMUSERSERVICE_H_
#define SRC_SERVICE_ABMUSERSERVICE_H_

#include "../dao/UserDao.h"
#include "../dao/MatchDao.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

/**
 * Clase que se utiliza para el ABM de usuarios en el server. Basicamente es
 * para poder almacenar los tokens para poder controlar la sesion del usuario.
 */
class AbmUserService {
private:
	UserDao* userDao;
	MatchDao* matchDao;
public:
	AbmUserService();
	/**
	 * Crea un nuevo usuario con password, token y email para un posible recupero
	 * de cuenta.
	 *
	 * @param string name
	 * @param string password
	 * @param string token
	 * @param string email
	 */
	string createNewUser(UserProfile* userProfile);
	/**
	 * Modificación del usuario. Puede modificar su password, mail y cualquier otro
	 * tipo de contenido relacionado con su perfil.
	 *
	 * Se disparará una excepción que puede ser MergeDbException en caso de no poder
	 * actualizar las credenciales.
	 *
	 * @param userProfile
	 * @throw MergeDbException,JsonParseException
	 */
	void modifyUser(UserProfile* userProfile)throw (InvalidEntityException);
	virtual ~AbmUserService();
};

#endif /* SRC_SERVICE_ABMUSERSERVICE_H_ */
