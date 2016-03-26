/*
 * User.h
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#ifndef USER_H_
#define USER_H_

#include <string>
#include "Entity.h"
#include "../exception/JsonParseException.h"
#include "../json/json.h"


class User : public Entity{
private:
	std::string name;
	std::string password;
	std::string firstName;
	std::string lastName;
	std::string email;
public:

	User(std::string json);

	User(std::string name, std::string password);
	virtual ~User();

	std::string toJson();


	const std::string& getName() const {
		return name;
	}

	void setName(const std::string& name) {
		this->name = name;
	}

	const std::string& getPassword() const {
		return password;
	}

	void setPassword(const std::string& password) {
		this->password = password;
	}

	const std::string& getFirstName() const {
		return firstName;
	}

	void setFirstName(const std::string& firstName) {
		this->firstName = firstName;
	}

	const std::string& getLastName() const {
		return lastName;
	}

	void setLastName(const std::string& lastName) {
		this->lastName = lastName;
	}

	const std::string& getEmail() const {
		return email;
	}

	void setEmail(const std::string& email) {
		this->email = email;
	}
};

#endif /* USER_H_ */
