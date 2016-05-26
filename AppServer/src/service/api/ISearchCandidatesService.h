#ifndef ISEARCHCANDIDATESSERVICE_H_
#define ISEARCHCANDIDATESSERVICE_H_

using namespace std;

class ISearchCandidatesService{

public:

	virtual ~ISearchCandidatesService(){};

	virtual list<UserProfile*> getCandidates(string idUser)=0;

};

#endif
