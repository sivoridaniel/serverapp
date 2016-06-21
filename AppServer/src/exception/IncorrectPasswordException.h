/*
 * IncorrectPasswordException.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_EXCEPTION_INCORRECTPASSWORDEXCEPTION_H_
#define SRC_EXCEPTION_INCORRECTPASSWORDEXCEPTION_H_

class IncorrectPasswordException: public std::exception
{
public:
	virtual const char* what() const throw ()
	{
		return "El password es incorrecto.";
	}
};

#endif /* SRC_EXCEPTION_INCORRECTPASSWORDEXCEPTION_H_ */
