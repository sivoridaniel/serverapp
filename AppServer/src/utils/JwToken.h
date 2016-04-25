/*
 * JwToken.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */


#include <sstream>
#include "jwt.h"
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
	string generarToken(string username) throw (TokenException);
	bool isTokenValid(string username) throw (TokenException);
	virtual ~JwToken();
private:

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
};


#endif /* SRC_UTILS_JWTOKEN_H_ */
