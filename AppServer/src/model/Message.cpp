/*
 * Message.cpp
 *
 *  Created on: May 20, 2016
 *      Author: agustin
 */

#include "Message.h"

Message::Message(long id, string idUser, string message)
{
	this->id = id;
	this->idUser = idUser;
	this->message = message;

}

Message::~Message()
{
}

string Message::toJson()
{
	Json::Value root;
	Json::FastWriter writer;
	root["message"]["id"] = std::to_string(this->id);
	root["message"]["user"] = this->idUser;
	root["message"]["value"] = this->message;
	std::string json = writer.write(root);
	return json;
}
