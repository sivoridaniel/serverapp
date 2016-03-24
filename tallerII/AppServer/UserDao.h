/*
 * UserDao.h
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#ifndef USERDAO_H_
#define USERDAO_H_
#include "rocksdb/db.h"
#include "User.h"
#include "EntityNotFoundException.h"
#include <exception>

class UserDao {

private:
	rocksdb::DB* db;

public:
	UserDao(rocksdb::DB* db);
	virtual ~UserDao();
	User* getUser(std::string name) throw(EntityNotFoundException);
	void putUser(User* user);

};

#endif /* USERDAO_H_ */
