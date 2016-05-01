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
	//TODO eliminar lo siguiente
	Match* match1 = new Match();
	match1->setIdUser("agustin");
	matchDao->put(match1);
	Match* match2 = new Match();
	match2->setIdUser("pablo");
	matchDao->put(match2);
	Match* match3 = new Match();
	match3->setIdUser("juan");
	matchDao->put(match3);
	Match* match4 = new Match();
	match4->setIdUser("juanma");
	matchDao->put(match4);
	delete match1;
	delete match2;
	delete match3;
	delete match4;
}

MatchService::MatchService(MatchDao* matchDao, ChatDao* chatDao){
	this->matchDao = matchDao;
	this->chatDao = chatDao;
}


MatchService::~MatchService() {
	delete matchDao;
	delete chatDao;
}

bool MatchService::addToYesList(string idUser, string idUserAccepted)  throw(IllegalStateException, EntityNotFoundException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	 Match* matchUser = (Match*)matchDao->get(idUser);
	 Match* matchUserAccepted = (Match*)matchDao->get(idUserAccepted);

	 if (matchUser->isRejected(idUserAccepted)){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<idUserAccepted<<" ya se encuentra en la lista de rechazados"));
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserAccepted)){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<idUserAccepted<<" ya se encuentra en la lista de matcheados"));
		 throw IllegalStateException();
	 }

	 //TODO: Verificar tambien que no exista en la lista de chats

	 // Si el otro usuario ya lo habia aceptado
	 // se quitan de sus listas de aceptados y se agregan a la lista de nuevos matchs
     if(matchUserAccepted->isAccepted(idUser)){
 		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Se registra un match entre "<<idUser<<" y "<<idUserAccepted ));

    	 matchUser->removeFromYesList(idUserAccepted);
    	 matchUser->addNewMatch(idUserAccepted);
    	 matchUserAccepted->removeFromYesList(idUser);
    	 matchUserAccepted->addNewMatch(idUser);
    	 matchDao->put(matchUser);
    	 matchDao->put(matchUserAccepted);
    	 // Como hubo match devolvemos true
    	 return true;
     }
     else if (!matchUser->isAccepted(idUserAccepted)){
    	 //Si el otro usuario no lo acepto aun, simplemente lo agregamos a la lista de aceptados
  		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Agregando el usuario "<<idUserAccepted<< " a la lista de aceptados de "<<idUser ));
		 matchUser->acceptUser(idUserAccepted);
		 matchDao->put(matchUser);
	 }

     //No hubo match aun, pero se agrego el usuario a la lista de si
     return false;
}

void MatchService::addToNoList(string idUser, string idUserRejected)  throw(IllegalStateException, EntityNotFoundException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	 Match* matchUser = (Match*)matchDao->get(idUser);

	 if (matchUser->isAccepted(idUserRejected)){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de aceptados"));
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserRejected)){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de matcheados"));
		 throw IllegalStateException();
	 }

	 if (matchUser->isRejected(idUserRejected)){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<idUserRejected<<" ya se encuentra en la lista de rechazados"));
		 return;
	 }

	 //TODO: Verificar tambien que no exista en la lista de chats
	 LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Agregando el usuario "<<idUserRejected<< " a la lista de rechazados de "<<idUser ));
	 matchUser->rejectUser(idUserRejected);
	 matchDao->put(matchUser);
}

list<string> MatchService::getNewMatches(string idUser) throw (EntityNotFoundException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Obteniendo la lista de nuevos matches del usuario "<<idUser ));

	Match* matchUser =  (Match*)matchDao->get(idUser);

	list<string> newMatches = matchUser->getNewMatches();

	return newMatches;
}

void MatchService::confirmUser(string idUser, string idUserConfirmed) throw(IllegalStateException, EntityNotFoundException){
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("MatchService"));

	 Match* matchUser = (Match*)matchDao->get(idUser);

	 if (matchUser->isAccepted(idUserConfirmed)){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<idUserConfirmed<<" ya se encuentra en la lista de aceptados"));
		 throw IllegalStateException();
	 }

	 if (matchUser->isRejected(idUserConfirmed)){
		LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("El usuario "<<idUserConfirmed<<" ya se encuentra en la lista de rechazados"));
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserConfirmed)){
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Confirmando match con usuario " << idUserConfirmed << " para usuario " <<idUser));
		 matchUser->removeFromNewMatches(idUser);
	 }
	 else{
		 throw IllegalStateException();
	 }

}




