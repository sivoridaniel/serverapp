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
#include "../exception/EntityNotFoundException.h"
#include "../exception/WriteDbException.h"

using namespace std;

/**
 * Singletos de la base rocksdb. Se genera por unica vez la instancia de la base y se utiliza
 * esta en los daos para poder realizar las operaciones correspondientes.
 *
 * ColumnFamilies definidas: USER 1
 *							 MATCH 2
 */
class DbHelper {
private:
	static rocksdb::DB* db;
	static std::vector<rocksdb::ColumnFamilyHandle*> handles;
public:
	/**
	 * Se crea la base si no esta creada y se crean las column families (collections) en caso
	 * de que estás no estén creadas. Luego se crean los handles para poder operar las column families.
	 *
	 * @param dbpath
	 */
	static void initDatabase(string dbpath);
	/**
	 * Se liberan los handles creados para poder operar las columns families.
	 *
	 */
	static void closeDatabase();
	/**
	 * Devuelve la instancia creada de rocksdb.
	 *
	 * @return rocksdb::DB*
	 */
	static rocksdb::DB* getDb(){
		return db;
	}

	/**
	 * Hace el put de la clave-valor en la columnFamily.
	 * Devuelve WriteDbException en cado de no poder realizar la operación exitosamente.
	 *
	 * @param string key (id)
	 * @param string value (json)
	 * @param int columnFamily
	 * @throw WriteDbException
	 */
	static void put(string id, string json, int columnFamily) throw(WriteDbException);

	/**
	 * Dada la clave (id) y la columnFamily donde se quiere hacer la consulta, se devuelve
	 * el json como resultado.
	 * Devuelve EntityNotFoundException en cado de no poder realizar la operación exitosamente.
	 *
	 * @param string id
	 * @param int columnFamily
	 * @throw EntityNotFoundException
	 * @return string json
	 *
	 */
	static string get(string id, int columnFamily) throw(EntityNotFoundException);
};

#endif /* DBHELPER_H_ */
