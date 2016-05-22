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
#include "Message.h"
#include <vector>

using namespace std;

class Chat : public Entity{
private:
	string idUser1;
	string idUser2;
	int lastSeenByUser1;
	int lastSeenByUser2;
	vector<Message*> messages;
public:
	Chat(){};
	Chat(string json);
	Chat(string idUser1, string idUser2);

	virtual ~Chat();

	string toJson();

	vector<Message*> getNewMessages(string idUser);

	void addNewMessage(string idUser, string message){
		if (idUser.compare(idUser1)==0){
			lastSeenByUser1 = messages.size()-1;
			Message* mess = new Message(lastSeenByUser1, idUser, message);
			messages.push_back(mess);
		}

		if (idUser.compare(idUser2)==0){
			lastSeenByUser2 = messages.size()-1;
			Message* mess = new Message(lastSeenByUser2, idUser, message);
			messages.push_back(mess);
		}
	}

	const string& getIdUser1() const {
		return idUser1;
	}

	void setIdUser1(const string& idUser1) {
		this->idUser1 = idUser1;
	}

	const string& getIdUser2() const {
		return idUser2;
	}

	void setIdUser2(const string& idUser2) {
		this->idUser2 = idUser2;
	}

	int getLastSeenByUser1() const {
		return lastSeenByUser1;
	}

	void setLastSeenByUser1(int lastSeenByUser1) {
		this->lastSeenByUser1 = lastSeenByUser1;
	}

	int getLastSeenByUser2() const {
		return lastSeenByUser2;
	}

	void setLastSeenByUser2(int lastSeenByUser2) {
		this->lastSeenByUser2 = lastSeenByUser2;
	}

	vector<Message*> getMessages()  {
		vector<Message*> mess;
		for (std::vector<Message*>::iterator it=messages.begin(); it!=messages.end(); ++it){
			Message* mes = *it;
			long id = mes->getId();
			string idUser  = mes->getIdUser();
			string content = mes->getMessage();
			Message* message = new Message(id,idUser,content);
			mess.push_back(message);
		}
		return mess;
	}

};

#endif /* CHAT_H_ */
