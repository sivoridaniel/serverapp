/*
 * MergeDbException.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: pablo
 */

#ifndef SRC_EXCEPTION_MERGEDBEXCEPTION_H_
#define SRC_EXCEPTION_MERGEDBEXCEPTION_H_

#include <exception>

class MergeDbException: public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "Error modificando en rocksdb";
	}
};



#endif /* SRC_EXCEPTION_MERGEDBEXCEPTION_H_ */
