#ifndef INVALIDENTITYEXCEPTION_H_
#define INVALIDENTITYEXCEPTION_H_
#include <exception>

class InvalidEntityException: public std::exception
{
public:
	virtual const char* what() const throw ()
	{
		return "La entidad no es del tipo valido";
	}
};

#endif /* INVALIDENTITYEXCEPTION_H_ */
