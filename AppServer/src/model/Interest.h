/*
 * Interest.h
 *
 *  Created on: Apr 8, 2016
 *      Author: agustin
 */

#ifndef INTEREST_H_
#define INTEREST_H_

#include <string>
#include "Entity.h"

class Interest: public Entity
{

private:
	std::string category;
	std::string value;
public:
	Interest(std::string category, std::string value);
	virtual ~Interest();

	std::string toJson();

	const std::string& getCategory() const
	{
		return category;
	}

	void setCategory(const std::string& category)
	{
		this->category = category;
	}

	const std::string& getValue() const
	{
		return value;
	}

	void setValue(const std::string& value)
	{
		this->value = value;
	}
};

#endif /* INTEREST_H_ */
