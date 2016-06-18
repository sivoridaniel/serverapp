/*
 * MatchService.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#include "MatchService.h"

MatchService::MatchService() {
	this->matchDao = new MatchDao();
	this->searchStatsDao = new SearchStatsDao();
	this->chatService = new ChatService();
	this->sharedService = new RemoteSharedService("http://shared-server-match.herokuapp.com");
}


MatchService::MatchService(MatchDao* matchDao, SearchStatsDao* searchStatsDao, IChatService* chatService, IRemote* sharedService) {
	this->matchDao = matchDao;
	this->searchStatsDao = searchStatsDao;
	this->chatService = chatService;
	this->sharedService = sharedService;
}

MatchService::~MatchService() {
	delete matchDao;
	delete searchStatsDao;
	delete chatService;
	delete sharedService;
}

bool MatchService::addToYesList(string idUser, string idUserAccepted) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	if (idUser.compare(idUserAccepted)==0){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Es el mismo usuario"));
		throw IllegalStateException();
	}
	try {

		Match* matchUser = (Match*) matchDao->get(idUser);
		Match* matchUserAccepted = (Match*) matchDao->get(idUserAccepted);

		if (matchUser->isRejected(idUserAccepted)) {
			delete matchUser;
			delete matchUserAccepted;
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserAccepted<<" ya se encuentra en la lista de rechazados"));
			throw EntityExistsException();
		}

		if (matchUser->isMatched(idUserAccepted)) {
			delete matchUser;
			delete matchUserAccepted;
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserAccepted<<" ya se encuentra en la lista de matcheados"));
			throw EntityExistsException();
		}

		//Verifica tambien que no exista en la lista de chats
		try{
			Chat* chat = chatService->getChat(idUser, idUserAccepted);
			delete chat;
			delete matchUser;
			delete matchUserAccepted;
			LOG4CPLUS_ERROR(logger,
								LOG4CPLUS_TEXT("Los usuarios ya estan matcheados"));
			throw EntityExistsException();
		}catch(EntityNotFoundException& e){
			//OK
		}

		// Si el otro usuario ya lo habia aceptado
		// se quitan de sus listas de aceptados y se agregan a la lista de nuevos matchs
		if (matchUserAccepted->isAccepted(idUser)) {
			LOG4CPLUS_INFO(logger,
					LOG4CPLUS_TEXT("Se registra un match entre "<<idUser<<" y "<<idUserAccepted ));

			matchUser->removeFromYesList(idUserAccepted);
			matchUser->addNewMatch(idUserAccepted);
			matchUserAccepted->removeFromYesList(idUser);
			matchUserAccepted->addNewMatch(idUser);
			matchDao->put(idUser, matchUser);
			matchDao->put(idUserAccepted,matchUserAccepted);
			SearchStats* stats = (SearchStats*)searchStatsDao->get("stats");
			stats->addLike(idUserAccepted);
			searchStatsDao->put("stats", stats);
			delete stats;
			delete matchUser;
			delete matchUserAccepted;
			return true;
		} else if (!matchUser->isAccepted(idUserAccepted)) {
			//Si el otro usuario no lo acepto aun, simplemente lo agregamos a la lista de aceptados
			LOG4CPLUS_INFO(logger,
					LOG4CPLUS_TEXT("Agregando el usuario "<<idUserAccepted<< " a la lista de aceptados de "<<idUser ));
			matchUser->acceptUser(idUserAccepted);
			matchDao->put(idUser, matchUser);
			SearchStats* stats = (SearchStats*)searchStatsDao->get("stats");
			stats->addLike(idUserAccepted);
			searchStatsDao->put("stats", stats);
			delete stats;
			delete matchUser;
			delete matchUserAccepted;
			return false;
		}
		delete matchUser;
		delete matchUserAccepted;
		//No hubo match aun, pero se agrego el usuario a la lista de si
		return false;

	} catch (EntityNotFoundException& e) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("No se encontro el usuario en la base"));
		throw e;
	} catch (EntityExistsException& e){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	} catch (exception& e) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}

}

void MatchService::addToNoList(string idUser, string idUserRejected){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	if (idUser.compare(idUserRejected)==0){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Es el mismo usuario"));
		throw IllegalStateException();
	}
	try {
		Match* matchUser = (Match*) matchDao->get(idUser);

		if (matchUser->isAccepted(idUserRejected)) {
			delete matchUser;
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de aceptados"));
			throw EntityExistsException();
		}

		if (matchUser->isMatched(idUserRejected)) {
			delete matchUser;
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de matcheados"));
			throw EntityExistsException();
		}

		if (matchUser->isRejected(idUserRejected)) {
			delete matchUser;
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de rechazados"));
			return;
		}

		//Verifica tambien que no exista en la lista de chats
		try{
			Chat* chat = chatService->getChat(idUser, idUserRejected);
			delete chat;
			delete matchUser;
			LOG4CPLUS_ERROR(logger,
								LOG4CPLUS_TEXT("Los usuarios ya estan matcheados"));
			throw EntityExistsException();
		}catch(EntityNotFoundException& e){
			//OK
		}

		LOG4CPLUS_INFO(logger,
				LOG4CPLUS_TEXT("Agregando el usuario "<<idUserRejected<< " a la lista de rechazados de "<<idUser ));
		matchUser->rejectUser(idUserRejected);
		matchDao->put(idUser, matchUser);
		delete matchUser;

	} catch (EntityExistsException& e) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}
	catch (EntityNotFoundException& e) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("No se encontro el usuario en la base"));
		throw e;
	}
	catch (exception& e) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}
}

list<UserProfile*> MatchService::getNewMatches(string idUser){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	LOG4CPLUS_INFO(logger,
			LOG4CPLUS_TEXT("Obteniendo la lista de nuevos matches del usuario "<<idUser ));

	list<UserProfile*> matches;

	try {

		Match* matchUser = (Match*) matchDao->get(idUser);

		list<string> newMatches = matchUser->getNewMatches();
		for (std::list< string >::iterator it=newMatches.begin(); it!=newMatches.end(); ++it){
			string idUserMatched = *it;
			try{
				UserProfile* user = sharedService->getUser(idUserMatched);
				matches.push_back(user);
			}catch(exception& e){
				LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			}
		}
		delete matchUser;
		return matches;

	} catch (EntityNotFoundException& e) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("No se encontro el usuario en la base"));
		throw e;
	} catch (exception& e) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}
}

list<UserProfile*> MatchService::getChats(string idUser){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	LOG4CPLUS_INFO(logger,
			LOG4CPLUS_TEXT("Obteniendo la lista de chats del usuario "<<idUser ));

	list<UserProfile*> chats;

	try {

		Match* matchUser = (Match*) matchDao->get(idUser);

		list<string> chatsUser = matchUser->getChats();
		for (std::list< string >::iterator it=chatsUser.begin(); it!=chatsUser.end(); ++it){
			string idUserChat = *it;
			try{
				UserProfile* user = sharedService->getUser(idUserChat);
				chats.push_back(user);
			}catch(exception& e){
				LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
			}
		}
		delete matchUser;
		return chats;

	} catch (EntityNotFoundException& e) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("No se encontro el usuario en la base"));
		throw e;
	} catch (exception& e) {
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
		throw e;
	}
}


void MatchService::confirmUser(string idUser, string idUserConfirmed){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	if (idUser.compare(idUserConfirmed)==0){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Es el mismo usuario"));
		throw IllegalStateException();
	}

	Match* matchUser = (Match*) matchDao->get(idUser);

	if (matchUser->isAccepted(idUserConfirmed)) {
		delete matchUser;
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El usuario "<<idUserConfirmed<<" ya se encuentra en la lista de aceptados"));
		throw EntityExistsException();
	}

	if (matchUser->isRejected(idUserConfirmed)) {
		delete matchUser;
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El usuario "<<idUserConfirmed<<" ya se encuentra en la lista de rechazados"));
		throw EntityExistsException();
	}

	if (matchUser->isMatched(idUserConfirmed)) {
		LOG4CPLUS_INFO(logger,
				LOG4CPLUS_TEXT("Confirmando match con usuario " << idUserConfirmed << " para usuario " <<idUser));
		matchUser->removeFromNewMatches(idUserConfirmed);
		matchUser->addChat(idUserConfirmed);
		matchDao->put(idUser,matchUser);
		try{
			chatService->createChat(idUser,idUserConfirmed);
			LOG4CPLUS_INFO(logger,
					LOG4CPLUS_TEXT("Se crea el chat entre el usuario " << idUserConfirmed << " y el usuario " <<idUser));
		}catch(EntityExistsException& e){
			LOG4CPLUS_WARN(logger,
					LOG4CPLUS_TEXT("Ya existe el chat entre el usuario " << idUserConfirmed << " y el usuario " <<idUser));
		}
		delete matchUser;
	} else {
		delete matchUser;
		throw IllegalStateException();
	}

}

bool MatchService::isACandidate(string idUser, string idOtherUser){
	Match* matchUser;
	Match* matchOtherUser;
	try{
	 matchUser = (Match*) matchDao->get(idUser);
	}catch(exception& e){
		return false;
	}
	try{
	 matchOtherUser = (Match*)matchDao->get(idOtherUser);
	}catch(exception& e){
		return false;
	}

	if (matchUser->isAccepted(idOtherUser)){
		delete matchUser;
		delete matchOtherUser;
		return false;
	}
	if (matchUser->isRejected(idOtherUser)){
		delete matchUser;
		delete matchOtherUser;
		return false;
	}
	if (matchUser->isMatched(idOtherUser)){
		delete matchUser;
		delete matchOtherUser;
		return false;
	}
	if (matchOtherUser->isRejected(idUser)){
		delete matchUser;
		delete matchOtherUser;
		return false;
	}
	try{
		Chat* chat = chatService->getChat(idUser, idOtherUser);
		delete chat;
		delete matchUser;
		delete matchOtherUser;
		return false;
	}catch(EntityNotFoundException& e){
		delete matchUser;
		delete matchOtherUser;
		return true;
	}
	return true;
}

