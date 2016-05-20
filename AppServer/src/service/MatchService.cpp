/*
 * MatchService.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: agustin
 */

#include "MatchService.h"

MatchService::MatchService() {
	this->matchDao = new MatchDao();
	this->chatDao = new ChatDao();
	this->sharedService = new RemoteSharedService("http://shared-server-match.herokuapp.com");
	try{
		matchDao->get("3");
	}catch(EntityNotFoundException& e){
		matchDao->put("3", new Match());
	}
	try{
		matchDao->get("4");
	}catch(EntityNotFoundException& e){
		matchDao->put("4", new Match());
	}
}

MatchService::MatchService(MatchDao* matchDao, ChatDao* chatDao, RemoteSharedService* sharedService) {
	this->matchDao = matchDao;
	this->chatDao = chatDao;
	this->sharedService = sharedService;
}

MatchService::~MatchService() {
	delete matchDao;
	delete chatDao;
	delete sharedService;
}

bool MatchService::addToYesList(string idUser, string idUserAccepted)
		throw (EntityExistsException, EntityNotFoundException) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	if (idUser.compare(idUserAccepted)==0){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Es el mismo usuario"));
		throw exception();
	}
	try {

		Match* matchUser = (Match*) matchDao->get(idUser);
		Match* matchUserAccepted = (Match*) matchDao->get(idUserAccepted);

		if (matchUser->isRejected(idUserAccepted)) {
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserAccepted<<" ya se encuentra en la lista de rechazados"));
			throw EntityExistsException();
		}

		if (matchUser->isMatched(idUserAccepted)) {
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserAccepted<<" ya se encuentra en la lista de matcheados"));
			throw EntityExistsException();
		}

		//TODO: Verificar tambien que no exista en la lista de chats

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
			// Como hubo match devolvemos true
			return true;
		} else if (!matchUser->isAccepted(idUserAccepted)) {
			//Si el otro usuario no lo acepto aun, simplemente lo agregamos a la lista de aceptados
			LOG4CPLUS_INFO(logger,
					LOG4CPLUS_TEXT("Agregando el usuario "<<idUserAccepted<< " a la lista de aceptados de "<<idUser ));
			matchUser->acceptUser(idUserAccepted);
			matchDao->put(idUser, matchUser);
		}

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

void MatchService::addToNoList(string idUser, string idUserRejected)
		throw (EntityExistsException, EntityNotFoundException) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	if (idUser.compare(idUserRejected)==0){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Es el mismo usuario"));
		throw exception();
	}
	try {
		Match* matchUser = (Match*) matchDao->get(idUser);

		if (matchUser->isAccepted(idUserRejected)) {
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de aceptados"));
			throw EntityExistsException();
		}

		if (matchUser->isMatched(idUserRejected)) {
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de matcheados"));
			throw EntityExistsException();
		}

		if (matchUser->isRejected(idUserRejected)) {
			LOG4CPLUS_ERROR(logger,
					LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de rechazados"));
			return;
		}

		//TODO: Verificar tambien que no exista en la lista de chats
		LOG4CPLUS_INFO(logger,
				LOG4CPLUS_TEXT("Agregando el usuario "<<idUserRejected<< " a la lista de rechazados de "<<idUser ));
		matchUser->rejectUser(idUserRejected);
		matchDao->put(idUser, matchUser);

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

list<UserProfile*> MatchService::getNewMatches(string idUser)
		throw (EntityNotFoundException) {
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

void MatchService::confirmUser(string idUser, string idUserConfirmed)
		throw (EntityExistsException, EntityNotFoundException) {
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	if (idUser.compare(idUserConfirmed)==0){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Es el mismo usuario"));
		throw exception();
	}

	Match* matchUser = (Match*) matchDao->get(idUser);

	if (matchUser->isAccepted(idUserConfirmed)) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El usuario "<<idUserConfirmed<<" ya se encuentra en la lista de aceptados"));
		throw EntityExistsException();
	}

	if (matchUser->isRejected(idUserConfirmed)) {
		LOG4CPLUS_ERROR(logger,
				LOG4CPLUS_TEXT("El usuario "<<idUserConfirmed<<" ya se encuentra en la lista de rechazados"));
		throw EntityExistsException();
	}

	if (matchUser->isMatched(idUserConfirmed)) {
		LOG4CPLUS_INFO(logger,
				LOG4CPLUS_TEXT("Confirmando match con usuario " << idUserConfirmed << " para usuario " <<idUser));
		matchUser->removeFromNewMatches(idUser);
	} else {
		throw IllegalStateException();
	}

}

