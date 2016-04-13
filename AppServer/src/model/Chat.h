/*
 * Chat.h
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#ifndef CHAT_H_
#define CHAT_H_

#include <string>
#include "Entity.h"

using namespace std;

class Chat : public Entity{
private:
	string idUser1;
	string idUser2;
	//TODO: agregar las listas de mensajes
public:
	Chat();
	virtual ~Chat();

	string toJson();
};

#endif /* CHAT_H_ */
