/*
 * Chat.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#include "Chat.h"

Chat::Chat(string json)
{
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(json.c_str(), root);
	if (!ok)
	{
		throw JsonParseException();
	}
	this->idUser1 = root["chat"].get("iduser1", "").asString();
	this->idUser2 = root["chat"].get("iduser2", "").asString();
	this->lastSeenByUser1 = std::stoi(root["chat"].get("lastseen1", "").asString());
	this->lastSeenByUser2 = std::stoi(root["chat"].get("lastseen2", "").asString());

	const Json::Value messages = root["chat"]["messages"];
	for (unsigned int i = 0; i < messages.size(); ++i)
	{
		long id = std::stol(messages[i]["id"].asString());
		string user = messages[i]["user"].asString();
		string value = messages[i]["value"].asString();
		Message* message = new Message(id, user, value);
		this->messages.push_back(message);
	}
}

Chat::Chat(string idUser1, string idUser2)
{
	this->idUser1 = idUser1;
	this->idUser2 = idUser2;
	this->lastSeenByUser1 = 0;
	this->lastSeenByUser2 = 0;
}

Chat::~Chat()
{
	for (vector<Message*>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		Message* message = *it;
		delete message;
	}
}

string Chat::toJson()
{
	Json::Value root;
	Json::Value vecInterests(Json::arrayValue);
	Json::FastWriter writer;

	root["chat"]["iduser1"] = this->idUser1;
	root["chat"]["iduser2"] = this->idUser2;
	root["chat"]["lastseen1"] = std::to_string(this->lastSeenByUser1);
	root["chat"]["lastseen2"] = std::to_string(this->lastSeenByUser2);
	int i = 0;
	for (vector<Message*>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		Message* message = *it;
		root["chat"]["messages"][i]["id"] = std::to_string(message->getId());
		root["chat"]["messages"][i]["user"] = message->getIdUser();
		root["chat"]["messages"][i]["value"] = message->getMessage();
		i++;
	}

	string json = writer.write(root);
	return json;
}

vector<Message*> Chat::getNewMessages(string idUser)
{
	int lastMessageSeen = 0;
	if (idUser.compare(idUser1) == 0)
	{
		lastMessageSeen = lastSeenByUser1;
	}

	if (idUser.compare(idUser2) == 0)
	{
		lastMessageSeen = lastSeenByUser2;
	}

	vector<Message*> mess;
	for (std::vector<Message*>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		Message* mes = *it;
		long id = mes->getId();
		if (lastMessageSeen < id)
		{
			string idUser = mes->getIdUser();
			string content = mes->getMessage();
			Message* message = new Message(id, idUser, content);
			mess.push_back(message);
		}
	}
	return mess;
}
