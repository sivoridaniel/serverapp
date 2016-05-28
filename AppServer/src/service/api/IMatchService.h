#ifndef IMATCHSERVICE_H_
#define IMATCHSERVICE_H_

using namespace std;

class IMatchService{

public:

	virtual ~IMatchService(){};

	virtual bool addToYesList(string idUser, string idUserAccepted)=0;

	virtual void addToNoList(string idUser, string idUserRejected)=0;

	virtual list<UserProfile*> getNewMatches(string idUser)=0;

	virtual void confirmUser(string idUser, string idUserConfirmed)=0;

	virtual bool isACandidate(string idUser, string idOtherUser)=0;

};

#endif
