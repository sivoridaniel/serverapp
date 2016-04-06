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

	User* usr = new User(user);

	return usr;
}

void UserDao::putUser(User* user) {

	std::string name = user->getName();
	std::string userJson = user->toJson();
	rocksdb::Status s = this->db->Put(rocksdb::WriteOptions(),name,userJson);
	if (!s.ok())
	{
		throw std::exception();
	}

}
