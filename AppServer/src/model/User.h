/*
 * User.h
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#ifndef USER_H_
#define USER_H_

#include <string>
#include <list>
#include "Entity.h"
#include "Interest.h"
#include "Location.h"
#include "../exception/JsonParseException.h"
#include "../json/json.h"


class User : public Entity{
private:
	std::string id;
	std::string name;
	std::string alias;
	std::string password;
	std::string email;
	std::string photoProfile;
	std::list<Interest*> interests;
	Location* location;
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

	const std::string& getEmail() const {
		return email;
	}

	void setEmail(const std::string& email) {
		this->email = email;
	}

	const std::string& getAlias() const {
		return alias;
	}

	void setAlias(const std::string& alias) {
		this->alias = alias;
	}

	const std::string& getId() const {
		return id;
	}

	void setId(const std::string& id) {
		this->id = id;
	}

	const std::list<Interest*>& getInterests() const {
		return interests;
	}

	void setInterests(const std::list<Interest*>& interests) {
		this->interests = interests;
	}

	Location* getLocation() const {
		return location;
	}

	void setLocation(Location* location) {
		this->location = location;
	}

	const std::string& getPhotoProfile() const {
		return photoProfile;
	}

	void setPhotoProfile(const std::string& photoProfile) {
		this->photoProfile = photoProfile;
	}

	void addInterest(Interest* interest);

};

#endif /* USER_H_ */
