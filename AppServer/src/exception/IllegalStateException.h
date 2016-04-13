/*
 * IllegalStateException.h
 *
 *  Created on: Mar 23, 2016
 *      Author: agustin
 */

#ifndef ILLEGALSTATEEXCEPTION_H_
#define ILLEGALSTATEEXCEPTION_H_
#include <exception>

class IllegalStateException: public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "La operacion a realizar genera un estado inconsistente";
	}
};

#endif /* ILLEGALSTATEEXCEPTION_H_ */
