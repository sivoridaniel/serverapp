#ifndef IREMOTE_H_
#define IREMOTE_H_

using namespace std;
/**
 * Clase que se utiliza para la llamada a los servicios externos a la aplicación (shared server). 
 * Se utiliza para poder mockear los servicios de esta clase.
 */
class IRemote{

public:

	virtual ~IRemote(){};

	virtual void createInterest(Interest* interest) = 0;

	virtual list<Interest*> getInterests() = 0;

	virtual list<UserProfile*> getUsers() = 0;

	virtual UserProfile* getUser(string id) = 0;

	virtual void createUser(UserProfile* userProfile) = 0;

	virtual void deleteUser(string id) = 0;

	virtual void updateUser(UserProfile* userProfile) = 0;

	virtual string getPhoto(string id) = 0;


};

#endif
