/*
 * AbstractController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_CONTROLLER_ABSTRACTCONTROLLER_H_
#define SRC_CONTROLLER_ABSTRACTCONTROLLER_H_

#include <string>
#include "../webserver/mongoose.h"
#include "../utils/JwToken.h"
#include "../service/AuthenticationService.h"
#include "../service/AbmUserService.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>


using namespace std;

class AbstractController {

public:
	virtual string connect(struct mg_connection *nc, struct http_message *hm)=0;

	string STATUS_NOK = "400";
	string STATUS_OK = "200";
	string STATUS_NOT_FOUND = "404";

	virtual string valid_session(struct http_message *hm){

		UserProfile* userProfile = NULL;
		AuthenticationService * authenticationService = new AuthenticationService();
		AbmUserService * abmUserService = new AbmUserService();
		Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("AbstractController"));

		try{

		mg_str* mg_token = mg_get_http_header(hm,"token");
		string token = string(mg_token->p,mg_token->len);

		/* Get Form variables */

		//mg_get_http_var(&hm->message, "token", vecToken, sizeof(vecToken));

		//mg_http_parse_header(&hm->mg_header,'token',vecToken,sizeof(vecToken));

		/* Call authentication service */


			LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("TOKEN RECIBIDO: "+token));

			bool isTokenValid = JwToken::isTokenValid(token);


			if(isTokenValid){
				LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("TOKEN VALIDO"));
				string username = JwToken::getUserName(token);
				token = JwToken::generarToken(username); //se genera el nuevo token, con el nuevo timestamp, renovando la sesion
				userProfile=authenticationService->getUserLogin(username,""); //se pasa el password en vacío
				userProfile->setToken(token);
				abmUserService->modifyUser(userProfile); //se modifica el usuario para asignarle el nuevo token
				delete userProfile;
				return token;
			}else{
				return "400";
			}



		}catch(exception & e){
			LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			return "400";
		}

		delete abmUserService;
		delete authenticationService;

		return "400";
	}

	virtual ~AbstractController(){};
};

#endif /* SRC_CONTROLLER_ABSTRACTCONTROLLER_H_ */
