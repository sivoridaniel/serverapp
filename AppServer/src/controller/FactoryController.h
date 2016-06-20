/*
 * FactoryController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_CONTROLLER_FACTORYCONTROLLER_H_
#define SRC_CONTROLLER_FACTORYCONTROLLER_H_

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#include "AbmUserController.h"
#include "MatchController.h"
#include "AuthenticationController.h"
#include "SearchCandidatesController.h"
#include "ChatController.h"
#include <string>

using namespace std;

/**
 * Factory de services.
 * Los services se instanciaran por unica vez (patrón singleton).
 *
 */
class FactoryController
{
private:

	static FactoryController* singletonFactoryCtrl;
	/*CONTROLLERS*/
	static AbmUserController* abmUserController;
	static MatchController* matchController;
	static AuthenticationController* authenticationController;
	static SearchCandidatesController* searchController;
	static ChatController* chatController;

	//Método que crea los controllers
	static void createControllers(string url);
public:
	FactoryController(void)
	{
	}
	;
	/**
	 * Se crea por unica vez la instancia de la fabrica de singletons, dado
	 * que tambien es un singleton.
	 *
	 * @return FactoryController*
	 */
	static FactoryController* getInstance(string url)
	{
		//createControllers();
		if (singletonFactoryCtrl == NULL)
		{
			singletonFactoryCtrl = new FactoryController();
			singletonFactoryCtrl->createControllers(url);
		}
		return singletonFactoryCtrl;
	}
	;
	/**
	 * Metodo para manejar los controllers de cada servicio.
	 *
	 * @param mg_connection*
	 * @param struct http_message*
	 * @param struct mg_serve_http_opts
	 */
	void connect(struct mg_connection *nc, struct http_message *hm, struct mg_serve_http_opts s_http_server_opts);
	~FactoryController(void);

};

#endif /* SRC_CONTROLLER_FACTORYCONTROLLER_H_ */
