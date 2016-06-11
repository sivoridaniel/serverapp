/*
 * UserProfileController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_CONTROLLER_ABMUSERCONTROLLER_H_
#define SRC_CONTROLLER_ABMUSERCONTROLLER_H_

#include "../service/impl/AbmUserService.h"
#include "AbstractController.h"
#include "../webserver/mongoose.h"
#include "../utils/UriParser.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

using namespace std;
using namespace log4cplus;

/**
 * Clase que maneja las peticiones del usuario por api rest
 * de creación de nuevo usuario, y obtención de un usuario en particular.
 * Usa el servicio AbmUserService.
 */
class AbmUserController: public AbstractController{
public:
	AbmUserController();
	/**
	 * Se maneja la uri de consulta o creación de nuevo usuario.
	 *
	 *	RETORNA EL STATUS:
	 *
	 *	200: OK
	 *	400: ERROR ESPECIFICO
	 *	404: ERROR DESCONOCIDO / NO SE ENCONTRO LA URI
	 *
	 * @param mg_connection*
	 * @param http_message*
	 * @result string
	 */
	string connect(struct mg_connection *nc, struct http_message *hm);
	virtual ~AbmUserController();
private:
	AbmUserService* abmService;
	/**
	 * Método que crea el usuario con las credenciales, y lo da de alta en el shared
	 * server.
	 *
	 * En caso satisfactorio retorna success true con el codigo del usuario que devuelve el
	 * shared server, y pone en el body del mensaje: {"success": "true", "data":"id"}
	 *
	 * Caso contrario devuelve el error indicando el mensaje por el cual no fue satisfactorio.
	 * {"success": "false", "data":"mensaje de error"}
	 *
	 * Retorna:
	 *
	 * 200: OK
	 * 400: ERROR (BAD REQUEST)
	 *
	 * @param struct mg_connection *nc
	 * @param struct http_message *hm
	 * @return string
	 */
	string event_handler_new_user(struct mg_connection *nc, struct http_message *hm);

	/**
	 * Actualiza el perfil del usuario.
	 *
	 * En caso satisfactorio se pone en el body del mensaje succes true con data profile update, en formato
	 * json:  {"success": "true", "data":"profile updated"}
	 *
	 * Caso contrario devuelve el error indicando el mensaje por el cual no se pudo actualizar el usuario.
	 * {"success": "false", "data":"mensaje de error"}
	 *
	 * Retorna:
	 *
	 * 200: OK
	 * 400: ERROR (BAD REQUEST)
	 * 404: ENTITY NOT FOUND
	 *
	 * @param struct mg_connection *nc
	 * @param struct http_message *hm
	 * @return string
	 *
	 */
	string event_handler_update_user(struct mg_connection *nc, struct http_message *hm);

	/**
	 * Obtiene la lista de intereses del shared server
	 *
	 *
	 * Retorna:
	 *
	 * 200: OK
	 * 400: ERROR (BAD REQUEST)
	 *
	 * @param struct mg_connection *nc
	 * @param struct http_message *hm
	 * @return string
	 *
	 */
	string event_handler_get_interests(struct mg_connection *nc, struct http_message *hm);

	/**
	 * Obtiene la foto del usuario del shared server
	 *
	 *
	 * Retorna:
	 *
	 * 200: OK
	 * 400: ERROR (BAD REQUEST)
	 *
	 * @param struct mg_connection *nc
	 * @param struct http_message *hm
	 * @return string
	 *
	 */
	string event_handler_get_photo(struct mg_connection *nc, struct http_message *hm);


	/**
	 * Transforma la lista de intereses devuelta por el shared a un json.
	 *
	 * @param struct mg_connection *nc
	 * @param struct http_message *hm
	 * @return string
	 *
	 */
	string createInterestsResponse(list<Interest*> intereses);

	string createPhotoResponse(string photo);


};

#endif /* SRC_CONTROLLER_ABMUSERCONTROLLER_H_ */
