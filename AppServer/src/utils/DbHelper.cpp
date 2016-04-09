/*
 * DbHelper.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: agustin
 */

#include "DbHelper.h"


void DbHelper::initDatabase(string dbpath){

	//std::string dbpath = "/tmp/testdb";

		  // open DB
		rocksdb::Options options;
		  options.create_if_missing = true;
		 options.create_missing_column_families = true;

		std::vector<rocksdb::ColumnFamilyDescriptor> column_families;
		// have to open default column family
		column_families.push_back(
				rocksdb::ColumnFamilyDescriptor(rocksdb::kDefaultColumnFamilyName,
						rocksdb::ColumnFamilyOptions()));
		// open the user profiles column family
		column_families.push_back(
				rocksdb::ColumnFamilyDescriptor("user_profiles",
						rocksdb::ColumnFamilyOptions()));
		// open the matches column family
		column_families.push_back(
				rocksdb::ColumnFamilyDescriptor("matches",
						rocksdb::ColumnFamilyOptions()));
		rocksdb::Status status = rocksdb::DB::Open(options, dbpath, column_families,
				&handles, &db);
		assert(status.ok());

}

void DbHelper::closeDatabase(){
	// close db
	for (auto handle : handles) {
	  delete handle;
	 }
	delete db;
}

rocksdb::DB* DbHelper::db;
std::vector<rocksdb::ColumnFamilyHandle*> DbHelper::handles;



