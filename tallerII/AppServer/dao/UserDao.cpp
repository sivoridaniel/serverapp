/*
 * UserDao.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#include "UserDao.h"

UserDao::UserDao(rocksdb::DB* db) {
	this->db = db;
}

UserDao::~UserDao() {
	this->db = NULL;
}

User* UserDao::getUser(std::string name) throw(EntityNotFoundException){
	std::string user;

	rocksdb::Status s = this->db->Get(rocksdb::ReadOptions(),name,&user);
	if (!s.ok())
	{
		throw EntityNotFoundException();
	}

/*	int ini = 0;
	int end = user.find(",", ini);
	std::string password = user.substr(ini, end-ini);
	ini = end+1;
	end = user.find(",", ini);
	std::string firstName = user.substr(ini, end-ini);
	ini = end+1;
	end = user.find(",", ini);
	std::string lastName = user.substr(ini, end-ini);
	ini = end+1;
	end = user.find(",", ini);
	std::string email = user.substr(ini);*/

	User* usr = new User(user);

	return usr;
}

void UserDao::putUser(User* user) {
	std::string name = user->getName();
	/*std::string password = user->getPassword();
	std::string firstName = user->getFirstName();
	std::string lastName = user->getLastName();
	std::string email = user->getEmail();
	std::string userStream = password + "," + firstName + "," + lastName + "," + email;*/
	std::string userJson = user->toJson();
	rocksdb::Status s = this->db->Put(rocksdb::WriteOptions(),name,userJson);
	if (!s.ok())
	{
		throw std::exception();
	}
}
