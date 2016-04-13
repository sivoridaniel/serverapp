#ifndef WRITEDBEXCEPTION_H_
#define WRITEDBEXCEPTION_H_
#include <exception>

class WriteDbException: public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "Error grabando en rocksdb";
	}
};

#endif /* WRITEDBEXCEPTION_H_ */
