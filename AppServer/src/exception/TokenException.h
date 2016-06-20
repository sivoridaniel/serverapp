/*
 * TokenException.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_EXCEPTION_TOKENEXCEPTION_H_
#define SRC_EXCEPTION_TOKENEXCEPTION_H_

#include <exception>

class TokenException: public std::exception
{
public:
	virtual const char* what() const throw ()
	{
		return "Error generando token";
	}
};

#endif /* SRC_EXCEPTION_TOKENEXCEPTION_H_ */
