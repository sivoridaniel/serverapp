#ifndef IAUTHSERVICE_H_
#define IAUTHSERVICE_H_

using namespace std;

class IAuthenticationService{

public:

	virtual ~IAuthenticationService(){};

	virtual UserProfile* getUserLogin(string username,string password) throw(IncorrectPasswordException, EntityNotFoundException) =0;


};

#endif





