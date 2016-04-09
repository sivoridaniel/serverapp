/*
 * AbstractController.h
 *
 *  Created on: 9 de abr. de 2016
 *      Author: pablo
 */
#include <string>
#include "../webserver/mongoose.h"

#ifndef SRC_CONTROLLER_ABSTRACTCONTROLLER_H_
#define SRC_CONTROLLER_ABSTRACTCONTROLLER_H_

using namespace std;

class AbstractController {
public:
	virtual string connect(struct mg_connection *nc, struct http_message *hm)=0;
};

#endif /* SRC_CONTROLLER_ABSTRACTCONTROLLER_H_ */
