#ifndef ICHATSERVICE_H_
#define ICHATSERVICE_H_

using namespace std;
/**
 * Clase que se utiliza para el Chat de usuarios. 
 * Se utiliza para poder mockear los servicios de esta clase.
 */
class IChatService
{

public:

	virtual ~IChatService()
	{
	}
	;

	virtual vector<Message*> getAllMessages(string idUser1, string idUser2)=0;

	virtual vector<Message*> getNewMessages(string idUser1, string idUser2)=0;

	virtual void addNewMessage(string idUser1, string idUser2, string message)=0;

	virtual void updateLastMessageSeen(string idUser1, string idUser2, int messageIndex)=0;

	virtual void createChat(string idUser1, string idUser2)=0;

	virtual Chat* getChat(string idUser1, string idUser2)=0;

};

#endif
