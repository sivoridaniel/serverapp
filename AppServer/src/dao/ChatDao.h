/*
 * ChatDao.h
 *
 *  Created on: Apr 10, 2016
 *      Author: agustin
 */

#ifndef CHATDAO_H_
#define CHATDAO_H_

#include "../model/Chat.h"
#include "GenericDao.h"

using namespace std;

/**
 * Para manejar las operaciones en la base de datos de los chats
 * de los usuarios.
 */
class ChatDao : public GenericDao{
public:
	ChatDao(){};
	virtual ~ChatDao(){};
	/**
	 * Se obtiene el chat con id pasado por parámetro.
	 * @param id
	 * @throw EntityNotFoundException
	 * @return Entity*
	 */
	Entity* get(string id);
	/**
	 * Se inserta el chat con el id que le corresponde.
	 *
	 * @param id
	 * @param entity
	 * @throw InvalidEntityException
	 */
	void put(string id, Entity* entity);
};

#endif /* CHATDAO_H_ */
