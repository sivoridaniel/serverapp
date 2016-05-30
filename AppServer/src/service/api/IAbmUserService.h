#ifndef IABMUSERSERVICE_H_
#define IABMUSERSERVICE_H_

using namespace std;

class IAbmUserService{

public:

	virtual ~IAbmUserService(){};

	virtual string createNewUser(UserProfile* userProfile)=0;

	virtual void modifyUser(UserProfile* userProfile) = 0;

	virtual void updateToken(UserProfile* userProfile) = 0;

};

#endif
