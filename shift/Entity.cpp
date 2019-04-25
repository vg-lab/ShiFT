/*
 * Copyright (c) 2016 GMRV/URJC/UPM.
 *
 * Authors: Pablo Toharia <pablo.toharia@upm.es>
 *
 * This file is part of Shift
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
#include "Entity.h"

namespace shift
{
  EntityGid Entity::_shiftEntityGid = 0;

  Entity::Entity( void )
  {
    _entityGid = ++_shiftEntityGid;
  }

  Entity::~Entity( void )
  {
  }

  EntityGid Entity::entityGid( void ) const
  {
    return _entityGid;
  }

  void Entity::entityGid( EntityGid entityGid_ )
  {
    _entityGid = entityGid_;
  }

  EntityGid Entity::shiftEntityGid( void )
  {
    return _shiftEntityGid;
  }

  void Entity::shiftEntityGid( EntityGid shiftEntityGid_,
    const bool compare )
  {
    if ( shiftEntityGid_ > _shiftEntityGid || !compare )
    {
      _shiftEntityGid = shiftEntityGid_;
    }
  }

  void Entity::autoUpdatePropertyWithRelatedEntities(
      const std::string& relName,
      const std::vector< std::string >& relatedEntitiesNames_,
      TAutoUpdatePropertyOp op_,
      const std::string& origPropertyLabel_,
      const std::string& destPropertyLabel_ )
  {
    const auto& rel =
      *( shift::EntitiesWithRelationships::entities( ).relationships( )[ relName ]->asOneToN( ));
    const auto& relEntitiesIt = rel.find( this->entityGid( ));
    if ( relEntitiesIt == rel.end( ))
      return;
    const auto& relEntities = relEntitiesIt->second;
    const auto& ents = shift::EntitiesWithRelationships::entities( );
    const auto& entsMap = ents.map( );
    fires::Objects objs;
    for ( const auto& relEntGid : relEntities )
    {
      const auto& relEntIt = entsMap.find( relEntGid.first );
      if ( relEntIt == entsMap.end( ))
      {
        continue;
      }
      else for ( const auto& relatedEntitiesName : relatedEntitiesNames_ )
      {
        const auto objectProperties = relEntIt->second;
        if ( objectProperties->typeName( ) == relatedEntitiesName &&
          ( objectProperties->hasProperty( origPropertyLabel_ ) ||
          op_ == TAutoUpdatePropertyOp::COUNT ))
        {
          objs.add( relEntIt->second );
        }
      }

    }
    RelationshipProperties::autoCalcProperty(
      op_, origPropertyLabel_, destPropertyLabel_, objs, *this );

  }

}
