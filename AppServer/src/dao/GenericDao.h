#ifndef GENERICDAO_H_
#define GENERICDAO_H_

#include "../exception/EntityNotFoundException.h"
#include "../exception/InvalidEntityException.h"
#include <exception>
#include "../model/Entity.h"
#include "../utils/DbHelper.h"
#include "../utils/ColumnFamilies.h"

using namespace std;

/**
 * Clase abstracta padre que implementan todos los DAOS para poder realizar las operaciones
 * en las entities correspondientes.
 */
class GenericDao
{

public:

	GenericDao()
	{
	}

	virtual ~GenericDao()
	{
	}
	/**
	 * Devuelve la entity de un usuario en particular, dada la clave de
	 * busqueda id.
	 *
	 * @param string id
	 * @throw EntityNotFoundException
	 * @return Entity
	 *
	 */
	virtual Entity* get(string id)=0;
	/**
	 * Persiste la entidad cuya estructura es (clave-valor).
	 *
	 * @param Entity
	 * @throw InvalidEntityException
	 */
	virtual void put(string id, Entity* entity)=0;

};

#endif /* GENERICDAO_H_ */
