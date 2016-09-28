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
#ifndef __SHIFT__ENTITIES__
#define __SHIFT__ENTITIES__

#include "Entity.h"
#include "Relationship.h"
#include <shift/api.h>
#include <assert.h>

namespace shift
{

  class Entities
    : public std::unordered_map< unsigned int, Entity* >
  {
  public:
    void add( Entity* entity )
    {
      assert( this->find( entity->entityGid( )) == this->end( ));
      ( *this )[ entity->entityGid( ) ] = entity;
    }
  };

  class EntitiesWithRelationships
    : public Entities
  {
  public:

    //! Relations are referenced by a name in a string
    typedef std::unordered_map< std::string, Relationship* > TRelationshipMap ;
    const TRelationshipMap& relationships( void ) const
    {
      return _relationships;
    }

    TRelationshipMap& relationships( void )
    {
      return _relationships;
    }
protected:

    //! Relations are referenced by a name in a string
    TRelationshipMap _relationships;
  };

}
#endif // __SHIFT__ENTITIES__
