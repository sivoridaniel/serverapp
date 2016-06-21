#ifndef ISEARCHCANDIDATESSERVICE_H_
#define ISEARCHCANDIDATESSERVICE_H_

using namespace std;
/**
 * Clase que se utiliza para la busqueda de usuarios candidatos. 
 * Se utiliza para poder mockear los servicios de esta clase.
 */
class ISearchCandidatesService
{

public:

	virtual ~ISearchCandidatesService()
	{
	}
	;

	virtual list<UserProfile*> getCandidates(string idUser)=0;

};

#endif
