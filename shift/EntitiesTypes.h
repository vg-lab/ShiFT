/*
 * Copyright (c) 2014-2016 GMRV/URJC/UPM.
 *
 * Authors: Pablo Toharia <pablo.toharia@upm.es>
 *
 * This file is part of ShiFT
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef __SHIFT__ENTITIES_TYPES__
#define __SHIFT__ENTITIES_TYPES__

#include "Entity.h"
#include "error.h"
#include <shift/api.h>
#include <vector>
#include <string>
#include <tuple>

namespace shift
{

  class EntitiesTypes
  {
  public:
    enum  { ENTITY_NAME = 0, OBJECT = 1, IS_SUBENTITY = 2, IS_INPUT = 3 };
    typedef std::vector< std::tuple< std::string, Entity*, bool, bool >> TEntitiesTypes;

    virtual ~EntitiesTypes( void ) {}
    const TEntitiesTypes& entitiesTypes( void ) const { return _entitiesTypes; };
    const TEntitiesTypes& inputsTypes( void ) const { return _entitiesTypes; };

	Entity* getEntityObject( std::string entityTypeName ){
	for(auto entityTuple : _entitiesTypes )
	{
		if ( entityTypeName ==  std::get<EntitiesTypes::ENTITY_NAME>( entityTuple ))
		{
			return std::get<EntitiesTypes::OBJECT>( entityTuple );
		}
	}
	  SHIFT_THROW("Entity type not recognised");
  }

  protected:
    TEntitiesTypes _entitiesTypes;
    TEntitiesTypes _inputsTypes;

  };

}

#endif
