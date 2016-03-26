#ifndef JSONPARSEEXCEPTION_H_
#define JSONPARSEEXCEPTION_H_
#include <exception>

class JsonParseException: public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "error parseando json";
	}
};

#endif /* ENTITYNOTFOUNDEXCEPTION_H_ */
