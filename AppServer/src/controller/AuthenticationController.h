/*
 * IAuthenticationController.h
 *
 * Interface para mockear
 *
 *  Created on: 19 de abr. de 2016
 *      Author: pablo
 */
#include <string>
#include "../service/impl/AuthenticationService.h"
#include "../service/impl/AbmUserService.h"
#include "../utils/JwToken.h"
#include "../webserver/mongoose.h"
#include "AbstractController.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#ifndef SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_
#define SRC_CONTROLLER_AUTHENTICATIONCONTROLLER_H_

using namespace std;
using namespace log4cplus;

/**
 * Maneja la api rest de authenticacion.
 * Usa los servicios de AuthenticationService y AbmUserService.
 */
class AuthenticationController:public AbstractController {

public:
	AuthenticationController();
	/**
	 * Maneja las llamadas de login pasandole el message con los paramatros
	 * necesarios para el logueo.
	 *
	 * @param mg_connection
	 * @param http_message
	 * @return string status error (200: OK, 400: NOK, 404: NOT_FOUND)
	 */
	string connect(struct mg_connection *nc, struct http_message *hm);
	virtual ~AuthenticationController();

private:
	AuthenticationService* authenticationService;
	AbmUserService* abmUserService;
	/**
	 * Se pasan por post el nombre de usuario y password en un json ({user:{name:string, password:string}})
	 * y se obtiene el mismo haciendo la consulta en el shared y obtienendo las credenciales (token) para devolver
	 * en el body el usuario consultado con su perfil, y en el header el token generado (valido por 60 segundos).
	 *
	 * En caso satisfactorio devuelve: {"user":"perfil del usuario (VER MODEL UserProfile)"}
	 *
	 * Caso contrario devuelve el error indicando el mensaje por el cual no fue satisfactorio.
	 * {"success": "false", "data":"mensaje de error"}
	 *
	 * Retorna:
	 *
	 * 200: OK
	 * 400: ERROR (BAD REQUEST)
	 * 404: ENTITY NOT FOUND
	 *
	 * @param mg_connection
	 * @param http_message
	 * @return string
	 */
	string event_handler_login_user(struct mg_connection *nc, struct http_message *hm);
	/**
	 * Se pasa el en header el token del usuario y se verifica si el mismo es correcto.
	 * De ser así devuelve {"success": "true", "data":"logged"}, caso contrario devolverá
	 * {"success": "false", "data":"mensaje error"}. Esto irá en el cuerpo del mensaje.
	 * En el encabezado del mensaje (header), en caso de ser satisfactorio se devolverá el token
	 * actualizado (se actualizará para poder estar logueado por 60 segundos extras).
	 *
	 * Retorna:
	 *
	 * 200: OK
	 * 400: ERROR (BAD REQUEST)
	 * 404: ENTITY NOT FOUND
	 *
	 * @param mg_connection
	 * @param http_message
	 * @return string
	 */
	string event_handler_valid_session(struct mg_connection *nc, struct http_message *hm);

	string createLoginResponse(UserProfile* user);
};



#endif /* SRC_CONTROLLER_IAUTHENTICATIONCONTROLLER_H_ */
