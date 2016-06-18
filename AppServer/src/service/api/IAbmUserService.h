#ifndef IABMUSERSERVICE_H_
#define IABMUSERSERVICE_H_

#include "../../model/Interest.h"

using namespace std;
/**
 * Clase que se utiliza para el ABM de usuarios en el server. 
 * Se utiliza para poder mockear los servicios de esta clase.
 */
 class IAbmUserService{

public:

	virtual ~IAbmUserService(){};

	virtual string createNewUser(UserProfile* userProfile)=0;

	virtual void modifyUser(UserProfile* userProfile) = 0;

	virtual void updateToken(UserProfile* userProfile) = 0;

	virtual string getPhoto(string id) = 0;

	virtual list<Interest*> getInterests() = 0;
};

#endif
