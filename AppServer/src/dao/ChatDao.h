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

class ChatDao : public GenericDao{
public:
	ChatDao();
	virtual ~ChatDao();
	Entity* get(std::string id) throw(EntityNotFoundException);
	void put(Entity* entity) throw(InvalidEntityException);
};

#endif /* CHATDAO_H_ */
