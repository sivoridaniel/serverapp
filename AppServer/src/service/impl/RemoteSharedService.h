/*
 * RemoteSharedService.h
 *
 *  Created on: May 14, 2016
 *      Author: agustin
 */

#ifndef REMOTESHAREDSERVICE_H_
#define REMOTESHAREDSERVICE_H_

#include "../../model/UserProfile.h"
#include "../../model/Interest.h"
#include "../../exception/EntityExistsException.h"
#include "../../exception/EntityNotFoundException.h"
#include "../../exception/RemoteException.h"
#include "../api/IRemote.h"

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
	/**
	 * Realiza el post del nuevo interes que se quiere agregar en el shared.
	 * En caso de fallar por ya existir o por algun error de formato u otro motivo en la llamada al servicio
	 * disparará la excepción que corresponda.
	 * 
	 * @throw EntityExistsException
	 * @throw RemoteException
	 */
	void createInterest(Interest* interest);

	/**
	 * Reliza el get de los intereses. En caso de no haber error en la llamada al servicio, devolverá en el body de la respuesta
	 * el json con todos los intereses creados en el shared. Estos intereses son procesados y retornados en una lista de intereses.
	 *
	 * @throw RemoteException
	 * @return list<Interest*>
	 */
	list<Interest*> getInterests();

	/**
	 * Reliza el get de los usuarios. En caso de no haber error en la llamada al servicio, devolvera en el body de la respuesta
	 * el json con todos los usuarios creados en el shared. Estos usuarios son procesados y retornados en una lista de usuarios.
	 *
	 * @throw RemoteException
	 * @return list<UserProfile*>
	 */
	list<UserProfile*> getUsers();

	/**
	 * Reliza el get del usuario con parametro id. En caso de no haber error en la llamada al servicio, devolvera en el body de la respuesta
	 * el json el usuario que se quiere obtener, lo cual se procede a transformarlo a la entidad model UserProfile.
	 * En caso de no existir el usuario buscado disparará la excepción EntityNotFoundException.
	 * En caso de producirse algun error en la llamada al servicio del shared disparará la excepción RemoteException.
	 *
	 * @throw RemoteException
	 * @throw EntityNotFoundException
	 * @return UserProfile
	 */
	UserProfile* getUser(string id);

	/**
	 * Realiza el post del usuario pasandole el json a traves del metodo toSharedJson de la entidad usuario UserProfile. 
	 * En caso de que el usuario ya exista, disparará EntityExistsException. En caso de que haya algun error en la invocación,
	 * se disparará la excepción RemoteException.
	 * En caso de que la llamada sea satisfacoria, el servicio devolverá en el body de la respuesta el id generado para el usuario que
	 * se creo. Este id es seteado a userProfile.
	 *
	 * @throw RemoteException
	 * @throw EntityExistsException
	 */
	void createUser(UserProfile* userProfile);

	/**
	 * Realiza el delete del usuario pasandole el parametro id. 
	 * En caso de que el usuario no exista, disparará EntityNotFoundException. En caso de que haya algun error en la invocación,
	 * se disparará la excepción RemoteException.
	 * 
	 * @throw RemoteException
	 * @throw EntityNotFoundException
	 */
	void deleteUser(string id);

	/**
	 * Realiza el update del usuario via put pasandole el json con los datos de la entidad UserProfile a traves del metodo 
	 * toSharedJson. 
	 * En caso de que el usuario no exista, disparará EntityNotFoundException. En caso de que haya algun error en la invocación,
	 * se disparará la excepción RemoteException.
	 * 
	 * @throw RemoteException
	 * @throw EntityNotFoundException
	 */
	void updateUser(UserProfile* userProfile);

private:

	/**
	 * Convierte el json en lista de Intereses.
	 * En caso de que el json este mal formado disparará JsonParseException.
	 * 
	 * @throw JsonParseException
	 * @return list<Interest*>
	 */
	list<Interest*> parseInterests(string json);
	/**
	 * Convierte el json en lista de Usuarios.
	 * En caso de que el json este mal formado disparará JsonParseException.
	 * 
	 * @throw JsonParseException
	 * @return list<UserProfile*>
	 */
	list<UserProfile*> parseUsers(string json);

};

#endif /* REMOTESHAREDSERVICE_H_ */
