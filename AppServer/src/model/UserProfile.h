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

using namespace std;

class UserProfile : public Entity{
private:
	string id;
	string name;
	string alias;
	string password;
	string token;
	string email;
	string photoProfile;
	list<Interest*> interests;
	Location* location;
public:

	UserProfile(string json);

	UserProfile(string name, string password);
	virtual ~UserProfile();

	string toJson();

	string toSharedJson();


	const string& getName() const {
		return name;
	}

	void setName(const string& name) {
		this->name = name;
	}

	const string& getPassword() const {
		return password;
	}

	void setPassword(const string& password) {
		this->password = password;
	}

	const string& getToken() const{
		return token;
	}

	void setToken(const string& token){
		this->token = token;
	}

	const string& getEmail() const {
		return email;
	}

	void setEmail(const string& email) {
		this->email = email;
	}

	const string& getAlias() const {
		return alias;
	}

	void setAlias(const string& alias) {
		this->alias = alias;
	}

	const string& getId() const {
		return id;
	}

	void setId(const string& id) {
		this->id = id;
	}

	const list<Interest*>& getInterests() const {
		return interests;
	}

	void setInterests(const list<Interest*>& interests) {
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

	void setPhotoProfile(const string& photoProfile) {
		this->photoProfile = photoProfile;
	}

	void addInterest(Interest* interest);

};

#endif /* USER_H_ */
