/*
 * EntityNotFoundException.h
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#ifndef ENTITYNOTFOUNDEXCEPTION_H_
#define ENTITYNOTFOUNDEXCEPTION_H_
#include <exception>

class EntityNotFoundException: public std::exception
{
public:
	virtual const char* what() const throw ()
	{
		return "No se encuentra la entidad";
	}
};

#endif /* ENTITYNOTFOUNDEXCEPTION_H_ */
