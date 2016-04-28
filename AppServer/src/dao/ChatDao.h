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

/**
 * Para manejar las operaciones en la base de datos de los chats
 * de los usuarios.
 */
class ChatDao : public GenericDao{
public:
	ChatDao();
	virtual ~ChatDao();

	Entity* get(std::string id) const throw(EntityNotFoundException);

	void put(Entity* entity) const throw(InvalidEntityException);
};

#endif /* CHATDAO_H_ */
