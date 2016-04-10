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

string MatchService::addToYesList(string idUser, string idUserAccepted)  throw(IllegalStateException){
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
    	 //TODO: Aca habria que devolver una respuesta al cliente para avisarle que hay un match
    	 // y que muestre la pantalla de confirmacion
     }
     else if (!matchUser->isAccepted(idUserAccepted)){
    	 //Si el otro usuario no lo acepto aun, simplemente lo agregamos a la lista de aceptados
		 matchUser->acceptUser(idUserAccepted);
		 matchDao->put(matchUser);
	 }


	 return "ok";
}

string MatchService::addToNoList(string idUser, string idUserRejected)  throw(IllegalStateException){
	 Match* matchUser = (Match*)matchDao->get(idUser);

	 if (matchUser->isAccepted(idUserRejected)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserRejected)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isRejected(idUserRejected)){
		 return "ok";
	 }

	 //TODO: Verificar tambien que no exista en la lista de chats

	 matchUser->rejectUser(idUserRejected);
	 matchDao->put(matchUser);
	 return "ok";
}

string MatchService::getNewMatches(string idUser){
	Match* matchUser =  (Match*)matchDao->get(idUser);

	list<string> newMatches = matchUser->getNewMatches();

	//TODO: armar json
	return "json";
}

string MatchService::confirmUser(string idUser, string idUserConfirmed) throw(IllegalStateException){
	 Match* matchUser = (Match*)matchDao->get(idUser);

	 if (matchUser->isAccepted(idUserConfirmed)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isRejected(idUserConfirmed)){
		 throw IllegalStateException();
	 }

	 if (matchUser->isMatched(idUserConfirmed)){
		 matchUser->removeFromNewMatches(idUser);
		 //TODO: Crear el chat si no existe
		 return "confirmado";
	 }
	 else{
		 return "error";
	 }

}




