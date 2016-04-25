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


using namespace std;

class AbstractController {
public:
	virtual string connect(struct mg_connection *nc, struct http_message *hm)=0;
	virtual ~AbstractController(){};
};

#endif /* SRC_CONTROLLER_ABSTRACTCONTROLLER_H_ */
