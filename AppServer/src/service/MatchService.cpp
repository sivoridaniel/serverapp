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

}

MatchService::~MatchService() {
	delete matchDao;
	delete chatDao;
}

bool MatchService::addToYesList(string idUser, string idUserAccepted)  throw(IllegalStateException){
	 Match* matchUser = (Match*)matchDao->get(idUser);
	 Match* matchUserAccepted = (Match*)matchDao->get(idUserAccepted);

	 if (matchUser->isRejected(idUserAccepted)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserAccepted)){
		 throw IllegalStateException();
	 }

	 //TODO: Verificar tambien que no exista en la lista de chats

	 // Si el otro usuario ya lo habia aceptado
	 // se quitan de sus listas de aceptados y se agregan a la lista de nuevos matchs
     if(matchUserAccepted->isAccepted(idUser)){
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
		 matchUser->acceptUser(idUserAccepted);
		 matchDao->put(matchUser);
	 }

     //No hubo match aun, pero se agrego el usuario a la lista de si
     return false;
}

void MatchService::addToNoList(string idUser, string idUserRejected)  throw(IllegalStateException){
	 Match* matchUser = (Match*)matchDao->get(idUser);

	 if (matchUser->isAccepted(idUserRejected)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserRejected)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isRejected(idUserRejected)){
		 return;
	 }

	 //TODO: Verificar tambien que no exista en la lista de chats

	 matchUser->rejectUser(idUserRejected);
	 matchDao->put(matchUser);
}

list<string> MatchService::getNewMatches(string idUser){
	Match* matchUser =  (Match*)matchDao->get(idUser);

	list<string> newMatches = matchUser->getNewMatches();

	return newMatches;
}

void MatchService::confirmUser(string idUser, string idUserConfirmed) throw(IllegalStateException){
	 Match* matchUser = (Match*)matchDao->get(idUser);

	 if (matchUser->isAccepted(idUserConfirmed)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isRejected(idUserConfirmed)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserConfirmed)){
		 matchUser->removeFromNewMatches(idUser);
	 }
	 else{
		 throw IllegalStateException();
	 }

}




