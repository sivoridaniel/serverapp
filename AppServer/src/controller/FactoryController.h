/*
 * FactoryController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */

#include "UserLoginController.h"
#include <string>

#ifndef SRC_CONTROLLER_FACTORYCONTROLLER_H_
#define SRC_CONTROLLER_FACTORYCONTROLLER_H_

#ifndef NULL
#define NULL   ((void *) 0)
#endif

using namespace std;

class FactoryController {
public:
	FactoryController(void);
	static FactoryController* getInstance();
	string connect(struct mg_connection *nc, struct http_message *hm);
	~FactoryController(void);
private:

	static FactoryController* singletonFactoryCtrl;
	/*CONTROLLERS*/
	static UserLoginController* userLoginController;

	//Método que crea los controllers
	void createControllers();
};

#endif /* SRC_CONTROLLER_FACTORYCONTROLLER_H_ */
