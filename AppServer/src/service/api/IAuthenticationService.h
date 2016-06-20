#ifndef IAUTHSERVICE_H_
#define IAUTHSERVICE_H_

using namespace std;
/**
 * Clase que se utiliza para manejar la autenticación de usuarios. 
 * Se utiliza para poder mockear los servicios de esta clase.
 */
class IAuthenticationService
{

public:

	virtual ~IAuthenticationService()
	{
	}
	;

	virtual UserProfile* getUserLogin(string username, string password) =0;

};

#endif

