/*
 * ChatService.h
 *
 *  Created on: May 20, 2016
 *      Author: agustin
 */

#ifndef CHATSERVICE_H_
#define CHATSERVICE_H_


#include "../../dao/MatchDao.h"
#include "../../dao/ChatDao.h"
#include "RemoteSharedService.h"
#include "../../exception/IllegalStateException.h"
#include "../../exception/EntityExistsException.h"
#include "../api/IChatService.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>
#include <vector>

using namespace std;
using namespace log4cplus;

class ChatService : public IChatService{
private:
	ChatDao* chatDao;
public:
	ChatService();
	ChatService(ChatDao* chatDao);
	virtual ~ChatService();

	/**
	 * Devuelve todos los mensajes de una conversacion
	 * @param string idUser1 uno de los usuarios de la conversacion
	 * @param string idUser2 el otro de los usuarios de la conversacion
	 * @throw EntityNotFoundException si no existe la conversacion o no existe alguno de los usuarios
	 */
	vector<Message*> getAllMessages(string idUser1, string idUser2);

	/**
	 * 	Devuelve los mensajes no leidos de un usuario con otro
	 *
	 * @param string idUser1 usuario que solicita los nuevos mensajes
	 * @param string idUser2 usuario del cual se piden sus nuevos mensajes
	 * @throw EntityNotFoundException si no existe la conversacion o no existe alguno de los usuarios
	 * @return vector<Message*> lista de los nuevos mensajes para el usuario idUser1 con el usuario idUser2
	 */
	vector<Message*> getNewMessages(string idUser1, string idUser2);

	/**
	 * Postea el mensaje del usuario idUser1 en la conversacion con el usuario idUser2
	 *
	 * @param string idUser1 emisor del mensaje
	 * @param string idUser2 receptor del mensaje
	 * @throw EntityNotFoundException si no existe la conversacion o no existe alguno de los usuarios
	 */
	void addNewMessage(string idUser1, string idUser2, string message);

	/**
	 * Actualiza el último mensaje visto del idUser1 de los mensajes enviados por idUser2
	 *
	 * @param string idUser1 usuario que desea notificar la actualizacion
	 * @param string idUser2 usuario con el cual tiene la conversacion el idUser1
	 * @throw EntityNotFoundException si no existe la conversacion o no existe alguno de los usuarios
	 */
	void updateLastMessageSeen(string idUser1, string idUser2, int messageIndex);

	/**
	 * Da de alta una nueva conversacion
	 * @param string idUser1 usuario participante
	 * @param string idUser2 usuario participante
	 * @throw EntityNotFoundException si no existe alguno de los usuarios
	 * @throw EntityExistsException si ya existe la conversacion
	 */
	void createChat(string idUser1, string idUser2);

	/**
	 * Obtiene la conversación de los usuarios participantes.
	 * @param string idUser1 usuario participante
	 * @param string idUser2 usuario participante
	 * @throw EntityNotFoundException si no existe alguno de los usuarios
	 */
	Chat* getChat(string idUser1, string idUser2);

private:

	/**
	 * Genera el id del chat en orden alfabetico.
	 * Si idUser1 >= idUser2 -> resultado idUser2 - idUser1, caso contrario
     * será idUser1 - idUser1 el id generado para la conversación.
	 * @param string idUser1 usuario participante
	 * @param string idUser2 usuario participante
	 */
	string generateChatId(string idUser1, string idUser2);

};

#endif /* CHATSERVICE_H_ */
