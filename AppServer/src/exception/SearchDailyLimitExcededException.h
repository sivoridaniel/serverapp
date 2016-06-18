#ifndef SEARCHDAILYLIMITEXCEDEDEXCEPTION_H_
#define SEARCHDAILYLIMITEXCEDEDEXCEPTION_H_
#include <exception>

class SearchDailyLimitExcededException: public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "se excedio el limite diario de busquedas de candidatos";
	}
};

#endif /* SEARCHDAILYLIMITEXCEDEDEXCEPTION_H_ */
