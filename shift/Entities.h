/*
 * Copyright (c) 2014-2016 GMRV/URJC/UPM.
 *
 * Authors: Pablo Toharia <pablo.toharia@upm.es>
 *          Iago Calvo Lista <i.calvol@alumnos.urjc.es>
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
#include "error.h"
#include <shift/api.h>
#include <unordered_map>
#include <vector>

namespace shift
{
  class Entity;
  class RelationshipOneToOne;
  class RelationshipOneToN;

  class Entities
  {
  public:
    // Entities indexed by its gid
    typedef unsigned int IndexedEntitiesKey;
    typedef Entity* IndexedEntitiesValue;
    typedef std::unordered_map< unsigned int, Entity* > IndexedEntities;
    // Entities vectorized to allow its use with fires tasks
    typedef std::vector< Entity* > VectorizedEntities;

    SHIFT_API
    void add( Entity* entity );

    SHIFT_API
    void addEntities( const Entities& entities );

    SHIFT_API
    void remove( const Entity* entity );

    SHIFT_API
    bool addIfNotContains( Entity* entity );

    SHIFT_API
    bool addEntitiesIfNotContains( const Entities& entities );

    SHIFT_API
    bool removeIfContains( const Entity* entity );

    SHIFT_API
    bool contains( const Entity* entity ) const;

    SHIFT_API
    bool empty( void ) const;

    SHIFT_API
    void addRelatedEntitiesOneToN( const RelationshipOneToN& relation,
      const Entity* entity, const Entities& searchEntities, int depthLevel = 0,
      Entities* compareEntities = nullptr, bool removeIfContained = true,
      bool removeContainedRelatives = true );

    SHIFT_API
    void addRelatedEntitiesOneToOne( const RelationshipOneToOne& relation,
      const Entity* entity, const Entities& searchEntities, int depthLevel = 0,
      Entities* compareEntities = nullptr, bool removeIfContained = true,
      bool removeContainedRelatives = true );

    SHIFT_API
    void removeRelatedEntitiesOneToN( const RelationshipOneToN& relation,
      const Entity* entity, const Entities& searchEntities,
      int depthLevel = 0 );

    SHIFT_API
    void removeRelatedEntitiesOneToOne( const RelationshipOneToOne& relation,
      const Entity* entity, const Entities& searchEntities,
      int depthLevel = 0 );

    IndexedEntitiesValue& at( const IndexedEntitiesKey& idx )
    {
      return _mapEntities.at( idx );
    }

    const IndexedEntitiesValue& at( const IndexedEntitiesKey& idx ) const
    {
      return _mapEntities.at( idx );
    }

    size_t size( void ) const
    {
      //SHIFT_CHECK_THROW( _vector.size( ) == _map.size( ),
      //  "ERROR: size incoherence between map and vector" );
      return _mapEntities.size( );
    }

    void clear( void )
    {
      _mapEntities.clear( );
      _vectorEntities.clear( );
    }

    const IndexedEntities& map( void ) const
    {
      return _mapEntities;
    }

    const VectorizedEntities& vector( void ) const
    {
      return _vectorEntities;
    }

    VectorizedEntities& vector( void )
    {
      return _vectorEntities;
    }

  protected:
    IndexedEntities _mapEntities;
    VectorizedEntities _vectorEntities;
  };

  class Relationship;

  class EntitiesWithRelationships
    : public Entities
  {
  public:
    //! Relations are referenced by a name in a string
    typedef std::unordered_map< std::string, Relationship* > TRelationshipMap;
    const TRelationshipMap& relationships( void ) const
    {
      return _relationships;
    }

    TRelationshipMap& relationships( void )
    {
      return _relationships;
    }
    //! Relations are referenced by a name in a string
    TRelationshipMap _relationships;

    SHIFT_API
    static shift::EntitiesWithRelationships& entities( void );

  private:
    static EntitiesWithRelationships _entities;

  };

}
#endif // __SHIFT__ENTITIES__
