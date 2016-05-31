#ifndef IAUTHSERVICE_H_
#define IAUTHSERVICE_H_

using namespace std;

class IAuthenticationService{

public:

	virtual ~IAuthenticationService(){};

	virtual UserProfile* getUserLogin(string username,string password) =0;


};

#endif





