#ifndef REMOTEEXCEPTION_H_
#define REMOTEEXCEPTION_H_
#include <exception>

class RemoteException: public std::exception
{
public:
	virtual const char* what() const throw ()
	{
		return "el shared server retorno error";
	}
};

#endif /* REMOTEEXCEPTION_H_ */
