/*
 * EntityExistsException.h
 *
 */

#ifndef ENTITYEXISTSEXCEPTION_H_
#define ENTITYEXISTSEXCEPTION_H_
#include <exception>

class EntityExistsException: public std::exception
{
public:
	virtual const char* what() const throw ()
	{
		return "La entidad ya existe";
	}
};

#endif /* ENTITYEXISTSEXCEPTION_H_ */
