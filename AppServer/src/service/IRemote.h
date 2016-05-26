#ifndef IREMOTE_H_
#define IREMOTE_H_

using namespace std;

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

};

#endif
