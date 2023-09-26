/*
 * Copyright (c) 2014-2016 VG-Lab/URJC/UPM.
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
    SHIFT_CHECK_THROW( _mapEntities.find( entityGid ) == _mapEntities.end( ),
      "ERROR: element already in map" );
    _mapEntities.insert( std::make_pair( entityGid, entity ));
    _vectorEntities.push_back( entity );
    SHIFT_CHECK_THROW( _vectorEntities.size( ) == _mapEntities.size( ),
      "ERROR: size incoherence between map and vector" );
  }

  void Entities::remove( const Entity* entity )
  {
    auto mapIt = _mapEntities.find( entity->entityGid( ));
    SHIFT_CHECK_THROW( mapIt != _mapEntities.end( ),
      "ERROR: element not contained in map" );
    _mapEntities.erase( mapIt );
    auto vectorIt = std::find( _vectorEntities.begin( ), _vectorEntities.end( ),
      entity );
    SHIFT_CHECK_THROW( vectorIt != _vectorEntities.end( ),
      "ERROR: element not contained in vector" );
    _vectorEntities.erase( vectorIt );
    SHIFT_CHECK_THROW(_vectorEntities.size( ) == _mapEntities.size( ),
      "ERROR: size incoherence between map and vector" );
  }

  bool  Entities::addIfNotContains( Entity* entity )
  {
    const auto entityGid = entity->entityGid( );
    auto mapIt = _mapEntities.find( entityGid );
    if ( mapIt == _mapEntities.end( ))
    {
      _mapEntities.insert( std::make_pair( entityGid, entity ));
      _vectorEntities.push_back( entity );
      SHIFT_CHECK_THROW( _vectorEntities.size( ) == _mapEntities.size( ),
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
    auto mapIt = _mapEntities.find( entity->entityGid( ));
    if ( mapIt != _mapEntities.end( ))
    {
      _mapEntities.erase( mapIt );
      auto vectorIt = std::find( _vectorEntities.begin( ),
        _vectorEntities.end( ), entity );
      //todo: performance may be improved by redoing _entitiesVector
      SHIFT_CHECK_THROW( vectorIt != _vectorEntities.end( ),
        "ERROR: element not contained in vector" );
      _vectorEntities.erase( vectorIt );
      SHIFT_CHECK_THROW(_vectorEntities.size( ) == _mapEntities.size( ),
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
    return _mapEntities.find( entity->entityGid( )) != _mapEntities.end( );
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

  void Entities::addRelatedEntitiesOneToOne(
    const RelationshipOneToOne& relation, const Entity* entity,
    const Entities& searchEntities, int depthLevel,
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
    const auto relationIt = relation.find(entity->entityGid( ));
    if(relationIt == relation.end( ))
    {
      return;
    }
    depthLevel--;
    for ( const auto& relative : relationIt->second )
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
    auto parent = entity->entityGid( );
    for ( auto relationIt = relation.find( parent );
          parent != 0 && relationIt != relation.end( ); --depthLevel )
    {
      parent = relationIt->first;
      Entity* relatedEntity = searchEntities.at( parent );
      if ( removeIfContains( relatedEntity ) && depthLevel != 1 )
      {
        relationIt = relation.find(parent);
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

  void Entities::addEntities( const Entities& entities )
  {
    for ( const auto entity : entities.vector( ))
    {
      auto entityGid = entity->entityGid( );
      SHIFT_CHECK_THROW( _mapEntities.find( entityGid ) == _mapEntities.end( ),
        "ERROR: element already in map" );
      _mapEntities.insert( std::make_pair( entityGid, entity ));
      _vectorEntities.push_back( entity );
    }
    SHIFT_CHECK_THROW( _vectorEntities.size( ) == _mapEntities.size( ),
      "ERROR: size incoherence between map and vector" );
  }

  bool Entities::addEntitiesIfNotContains( const Entities& entities )
  {
    bool returnValue = true;
    for ( const auto entity : entities.vector( ))
    {
      auto entityGid = entity->entityGid( );
      auto mapIt = _mapEntities.find( entityGid );
      if( mapIt == _mapEntities.end( ) )
      {
        _mapEntities.insert( std::make_pair( entityGid, entity ) );
        _vectorEntities.push_back( entity );
      }
      else
      {
        returnValue = false;
      }
    }
    SHIFT_CHECK_THROW( _vectorEntities.size( ) == _mapEntities.size( ),
      "ERROR: size incoherence between map and vector" );
    return returnValue;
  }
}
