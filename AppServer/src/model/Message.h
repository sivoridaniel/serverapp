/*
 * Message.h
 *
 *  Created on: May 20, 2016
 *      Author: agustin
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include "Entity.h"

using namespace std;

class Message: public Entity
{
private:
	long id;
	string idUser;
	string message;
public:
	Message(long id, string idUser, string message);

	virtual ~Message();

	string toJson();

	const string& getIdUser() const
	{
		return idUser;
	}

	void setIdUser(const string& idUser)
	{
		this->idUser = idUser;
	}

	const string& getMessage() const
	{
		return message;
	}

	void setMessage(const string& message)
	{
		this->message = message;
	}

	const long getId() const
	{
		return id;
	}

	void setId(long id)
	{
		this->id = id;
	}
};

#endif /* MESSAGE_H_ */
