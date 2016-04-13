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
#include <string>


using namespace std;

class FactoryController {
private:

	static FactoryController* singletonFactoryCtrl;
	/*CONTROLLERS*/
	static AbmUserController* abmUserController;
	static MatchController* matchController;

	//Método que crea los controllers
	static void createControllers();
public:
	FactoryController(void){};
	static FactoryController* getInstance(){
		//createControllers();
		if(singletonFactoryCtrl == NULL){
			singletonFactoryCtrl = new FactoryController();
			singletonFactoryCtrl->createControllers();
		}
		return singletonFactoryCtrl;
	};
	void connect(struct mg_connection *nc, struct http_message *hm, struct mg_serve_http_opts s_http_server_opts);
	~FactoryController(void);

};

#endif /* SRC_CONTROLLER_FACTORYCONTROLLER_H_ */
