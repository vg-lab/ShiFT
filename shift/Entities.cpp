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
#include "Entities.h"
#include "error.h"


namespace shift
{

  EntitiesWithRelationships EntitiesWithRelationships::_entities =
    EntitiesWithRelationships( );

  EntitiesWithRelationships& EntitiesWithRelationships::entities( void )
  {
    return _entities;
  }

  void Entities::add( Entity* entity )
  {
    const auto entityGid = entity->entityGid( );
    SHIFT_CHECK_THROW( _map.find( entityGid ) == _map.end( ),
      "ERROR: element already in map" );
    _map.insert( std::make_pair( entityGid, entity ));
    _vector.push_back( entity );
    SHIFT_CHECK_THROW( _vector.size( ) == _map.size( ),
      "ERROR: size incoherence between map and vector" );
  }

  void Entities::remove( const Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ));
    SHIFT_CHECK_THROW( mapIt != _map.end( ),
      "ERROR: element not contained in map" );
    _map.erase( mapIt );
    auto vectorIt = std::find( _vector.begin( ), _vector.end( ), entity );
    SHIFT_CHECK_THROW( vectorIt != _vector.end( ),
      "ERROR: element not contained in vector" );
    _vector.erase( vectorIt );
    SHIFT_CHECK_THROW(_vector.size( ) == _map.size( ),
      "ERROR: size incoherence between map and vector" );
  }

  bool  Entities::addIfNotContains( Entity* entity )
  {
    const auto entityGid = entity->entityGid( );
    auto mapIt = _map.find( entityGid);
    if ( mapIt == _map.end( ))
    {
      _map.insert( std::make_pair( entityGid, entity ));
      _vector.push_back( entity );
      SHIFT_CHECK_THROW( _vector.size( ) == _map.size( ),
        "ERROR: size incoherence between map and vector" );
      return true;
    }
    else
    {
      return false;
    }
  }

  bool  Entities::removeIfContains( const Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ));
    if ( mapIt != _map.end( ))
    {
      _map.erase( mapIt );
      auto vectorIt = std::find( _vector.begin( ),_vector.end( ), entity );
      SHIFT_CHECK_THROW( vectorIt != _vector.end( ),
        "ERROR: element not contained in vector" );
      _vector.erase( vectorIt );
      SHIFT_CHECK_THROW(_vector.size( ) == _map.size( ),
        "ERROR: size incoherence between map and vector" );
      return true;
    }
    else
    {
      return false;
    }
  }

  bool  Entities::contains( const Entity* entity ) const
  {
    return _map.find( entity->entityGid( )) != _map.end( );
  }

  void Entities::addRelatedEntitiesOneToN( const RelationshipOneToN& relation,
    const Entity* entity, const Entities& searchEntities, int depthLevel,
    Entities* compareEntities, const bool removeIfContained,
    const bool removeContainedRelatives )
  {
    auto relativesIt = relation.find( entity->entityGid( ));
    if(relativesIt == relation.end( ))
    {
      return;
    }
    const auto& relatives = relativesIt->second;
    depthLevel--;
    for ( const auto& relative : relatives )
    {
      Entity* relatedEntity = searchEntities.at( relative.first );
      bool contained = compareEntities && ( ( removeIfContained )
        ? compareEntities ->removeIfContains( relatedEntity )
        : compareEntities->contains( relatedEntity ));

      if ( contained  )
      {
        if ( removeContainedRelatives && compareEntities )
        {
          compareEntities->removeRelatedEntitiesOneToN( relation, relatedEntity,
            searchEntities, depthLevel );
        }
      }
      else if ( addIfNotContains( relatedEntity ) && depthLevel != 0 )
      {
        addRelatedEntitiesOneToN( relation, relatedEntity, searchEntities,
          depthLevel, compareEntities, removeIfContained );
      }
    }
  }

  void Entities::addRelatedEntitiesOneToOne( const RelationshipOneToOne& relation,
    const Entity* entity, const Entities& searchEntities, int depthLevel,
    Entities* compareEntities, const bool removeIfContained,
    const bool removeContainedRelatives )
  {
    auto parentIt = relation.find( entity->entityGid( ));
    while ( parentIt != relation.end( ))
    {
      auto parentGid = parentIt->second.entity;
      if ( parentGid == 0 )
      {
        return;
      }
      Entity* relatedEntity = searchEntities.at( parentGid );

      if( compareEntities != nullptr && (( removeIfContained )
        ? compareEntities->removeIfContains( relatedEntity )
        : compareEntities->contains( relatedEntity )))
      {
        //The entity it's contained
        if ( removeContainedRelatives && compareEntities )
        {
          compareEntities->removeRelatedEntitiesOneToOne( relation,
           relatedEntity, searchEntities, depthLevel );
        }
        return;
      }
      else if( addIfNotContains( relatedEntity ) && depthLevel != 1 )
      {
        parentIt = relation.find( parentGid );
      }
      else
      {
        return;
      }
      depthLevel--;
    }
  }

  void Entities::removeRelatedEntitiesOneToN( const RelationshipOneToN& relation,
    const Entity* entity, const Entities& searchEntities, int depthLevel)
  {
    const auto &relatives = relation.at( entity->entityGid( ));
    depthLevel--;
    for ( const auto& relative : relatives )
    {
      Entity* relatedEntity = searchEntities.at( relative.first );

      if ( removeIfContains( relatedEntity ) && depthLevel != 0 )
      {
        removeRelatedEntitiesOneToN( relation, relatedEntity, searchEntities,
          depthLevel);
      }
    }
  }

  void Entities::removeRelatedEntitiesOneToOne(
    const RelationshipOneToOne& relation, const Entity* entity,
    const Entities& searchEntities, int depthLevel )
  {
    for ( auto parent = relation.at( entity->entityGid( )).entity;
          parent != 0; --depthLevel )
    {
      Entity* relatedEntity = searchEntities.at( parent );
      if ( removeIfContains( relatedEntity ) && depthLevel != 1 )
      {
        parent = relation.at( parent ).entity;
      }
      else
      {
        return;
      }
    }
  }

  bool Entities::empty( void ) const
  {
    return vector( ).empty( );
  }
}
