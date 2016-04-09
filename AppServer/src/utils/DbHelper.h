/*
 * DbHelper.h
 *
 *  Created on: Apr 9, 2016
 *      Author: agustin
 */

#ifndef DBHELPER_H_
#define DBHELPER_H_

#include <assert.h>
#include <string>
#include "rocksdb/db.h"


using namespace std;

class DbHelper {
private:
	static rocksdb::DB* db;
	static std::vector<rocksdb::ColumnFamilyHandle*> handles;
public:
	static void initDatabase(string dbpath);
	static void closeDatabase();

	static rocksdb::DB* getDb(){
		return db;
	}
	static std::vector<rocksdb::ColumnFamilyHandle*> getHandles(){
		return handles;
	}
};

#endif /* DBHELPER_H_ */
