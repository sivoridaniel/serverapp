/*
 * JwToken.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */


#include <sstream>
#include <jwt.h>
#include <string>
#include "../exception/TokenException.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#ifndef SRC_UTILS_JWTOKEN_H_
#define SRC_UTILS_JWTOKEN_H_

using namespace std;
using namespace log4cplus;

class JwToken{

public:
	JwToken();
	/**
	 * Se utilizará para la generación del token el algoritmo JWT_ALG_HS256 utilizando una clave interna de 32 bytes. El header quedará
	 * conformado por el type: jwt (metodo de generación del token) y alg: HS256 que es el algoritmo utilizado.
	 * Para el payload se utiliza el nombre de usuario y el timestamp generado al momento de llamada a este metodo (que se da cuando
	 * el usuario se loguea al sistema). También agregamos en el payload el expire token dado en segundos (seteado en 60 segundos -> 1 min
	 * para mantener la sesión de usuario).
	 * El tercer y ultimo campo es la firma del token. En este campo se utiliza la clave interna de 32 bytes.
	 * El token que devuelve estara conformado de tres secciones separadas por puntos (HEADER.PAYLOAD.SIGNATURE), y cifradas en HS256.
	 *
	 * En caso de no poder generar alguna de las secciones (HEADER, PAYLOAD o SIGNATURE) se logueara el mensaje de error correspondiente y
	 * luego disparará la excepción TokenException para tratarla más arriba.
	 *
	 * @param string username
	 * @throw TokenException
	 * @return string token
	 */
	string generarToken(string username) throw (TokenException);
	/**
	 * Se verifica que el token no haya expirado. Para ello se decodifica el token y se evalua el timestamp que no haya superado los
	 * 60 segundos.
	 *
	 * @param string token
	 * @throw TokenException
	 * @return bool isTokenValid
	 */
	bool isTokenValid(string token) throw (TokenException);

	/**
	 * Para obtener el nombre de usuario del token, dado que el usuario enviará el token con su nombre
	 * en la sección payload cifrada en HS256.
	 *
	 * @param string token
	 * @throw TokenException en caso de no poder obtener el nombre de usuario
	 * @result string username
	 *
	 */
	string getUserName(string token)throw (TokenException);

	virtual ~JwToken();

private:

	/**
	 *  Se evalua la operación intcod y pcharcod para devolver una excepcion de token y loguear el mensaje correspondiente.
	 *  Esta función se utiliza de apoyo dado que hay algunos metodos de jwt que retornan intcod 0 si salio todo bien o pcharcod distinto
	 *  de NULL para otro tipo de operación que devuelve el char* final (ejemplo: la función que genera el token final).
	 *
	 *	Se disparará la excepción TokenException en caso de que intcod!=0 o pcharcod==NULL.
	 *
	 *	@param intcod type int
	 *	@param pcharcod type char*
	 *	@param msgerror type string
	 *	@param logger type Logger
	 *	@throw TokenException
	 *
	 */
	void evaluateOperation(int intcod,  char *pcharcod, string msgError,Logger logger)throw (TokenException);

	/** MSG_ERRORS **/
	static const string MSG_ERROR_NEW_JWT(){
		return "Error creando el objeto jwt";
	}
	static const string MSG_ERROR_HEADER(){
		return "Error generando el header";
	}
	static const string MSG_ERROR_PAYLOAD(){
		return "Error generando el payload";
	}
	static const string MSG_ERROR_TOKEN(){
		return "Error generando el token";
	}
	static const string MSG_ERROR_DECODE_TOKEN(){
			return "Error decodificando el token";
	}
	static const string MSG_ERROR_DECODE_USERNAME(){
			return "Error decodificando el username";
	}
};


#endif /* SRC_UTILS_JWTOKEN_H_ */
