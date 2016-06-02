/*
 * ChatDao.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#include "ChatDao.h"


Entity* ChatDao::get(string id){
	string json;

	json = DbHelper::get(id, CHAT);

	Chat* chat = new Chat(json);

	return chat;
}
void ChatDao::put(string id, Entity* e){
	Chat* chat = dynamic_cast<Chat*>(e);
	if (chat==0){
		throw InvalidEntityException();
	}

	string json = chat->toJson();

	DbHelper::put(id,json,CHAT);
}
