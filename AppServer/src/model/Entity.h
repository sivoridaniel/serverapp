#ifndef ENTITY_H_
#define ENTITY_H_

#include "../exception/JsonParseException.h"
#include "../json/json.h"

class Entity{

public:
	virtual std::string toJson()=0;

};

#endif /* ENTITY_H_ */
