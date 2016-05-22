/*
 * ChatService.cpp
 *
 *  Created on: May 20, 2016
 *      Author: agustin
 */

#include "ChatService.h"

ChatService::ChatService() {
	this->chatDao = new ChatDao();
}

ChatService::ChatService(ChatDao* chatDao) {
	this->chatDao = chatDao;
}

ChatService::~ChatService() {
	delete chatDao;
}

string ChatService::generateChatId(string idUser1, string idUser2){
	string id = "";
	if (idUser1.compare(idUser2) >= 0){
		id = idUser2+"-"+idUser1;
	}
	else{
		id = idUser1+"-"+idUser2;
	}
	return id;
}


vector<Message*> ChatService::getAllMessages(string idUser1, string idUser2) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatService"));
	string id = generateChatId(idUser1,idUser2);
	vector<Message*> messages;
	try{
		Chat* chat = (Chat*)chatDao->get(id);
		messages = chat->getMessages();
		delete chat;
	}catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("No se encuentra el chat "<<id));
		throw e;
	}
	return messages;
}


vector<Message*> ChatService::getNewMessages(string idUser1, string idUser2) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatService"));
	string id = generateChatId(idUser1,idUser2);
	vector<Message*> messages;
	try{
		Chat* chat = (Chat*)chatDao->get(id);
		messages = chat->getNewMessages(idUser1);
		delete chat;
	}catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("No se encuentra el chat "<<id));
		throw e;
	}
	return messages;
}


void ChatService::addNewMessage(string idUser1, string idUser2, string message) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatService"));
	string id = generateChatId(idUser1,idUser2);
	vector<Message*> messages;
	try{
		Chat* chat = (Chat*)chatDao->get(id);
		chat->addNewMessage(idUser1, message);
		delete chat;
	}catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("No se encuentra el chat "<<id));
		throw e;
	}
}


void ChatService::updateLastMessageSeen(string idUser1, string idUser2, int messageIndex){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatService"));
	string id = generateChatId(idUser1,idUser2);
	vector<Message*> messages;
	try{
		Chat* chat = (Chat*)chatDao->get(id);
		chat->setLastSeenByUser1(messageIndex);
		delete chat;
	}catch(EntityNotFoundException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("No se encuentra el chat "<<id));
		throw e;
	}
}


void ChatService::createChat(string idUser1, string idUser2) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("ChatService"));
	Chat* chat = new Chat(idUser1, idUser2);
	string id = generateChatId(idUser1, idUser2);
	//TODO: chequear que existan los usuarios
	try{
		try{
			chatDao->get(id);
			throw EntityExistsException();
		}catch(EntityNotFoundException& e){
			chatDao->put(id, chat);
		}
	}catch(EntityExistsException& e){
		LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("La conversacion "<<id<<" ya existe"));
		throw e;
	}catch(exception& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Error persisitiendo en la base"));
		throw e;
	}

}

Chat* ChatService::getChat(string idUser1, string idUser2) {

	string id = generateChatId(idUser1, idUser2);
	try{
		Chat* chat = (Chat*)chatDao->get(id);
		return chat;

	}catch(EntityNotFoundException& e){
		throw e;
	}
}

