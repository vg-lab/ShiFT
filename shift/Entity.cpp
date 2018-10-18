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
  static EntityGid shiftEntityGid = 0;

  Entity::Entity( void )
  {
    _entityGid = ++shiftEntityGid;
  }

  Entity::~Entity( void )
  {
  }

  EntityGid Entity::entityGid( void ) const
  {
    return _entityGid;
  }

  void Entity::autoUpdatePropertyWithRelatedEntities(
      const std::string& relName,
      const std::vector< std::string >& relatedEntitiesNames,
      TAutoUpdatePropertyOp op,
      const std::string& origPropertyLabel,
      const std::string& destPropertyLabel )
  {
    const auto& rel =
      *( shift::EntitiesWithRelationships::entities( ).relationships( )[ relName ]->asOneToN( ));
    const auto& relEntitiesIt = rel.find( this->entityGid( ));
    if ( relEntitiesIt == rel.end( ))
      return;
    const auto& relEntities = relEntitiesIt->second;
    const auto& ents = shift::EntitiesWithRelationships::entities( );
    fires::Objects objs;
    for ( const auto& relEntGid : relEntities )
    {
      const auto& entsMap = ents.map( );
      const auto& relEntIt = entsMap.find( relEntGid.first );
      if ( relEntIt == entsMap.end( )) continue;
      for ( const auto& relatedEntitiesName : relatedEntitiesNames )
        if ( relEntIt->second->entityName( ) == relatedEntitiesName )
          if ( relEntIt->second->hasProperty( origPropertyLabel ) ||
               op == TAutoUpdatePropertyOp::COUNT )
            objs.add( relEntIt->second );
    }

    if ( op == TAutoUpdatePropertyOp::COUNT )
    {
      this->setProperty( destPropertyLabel, uint( objs.size( )));
      return;
    }

    auto aggregator = fires::PropertyManager::getAggregator( origPropertyLabel );
    fires::Aggregate aggregate;
    fires::AggregateConfig aggregateConfig;
    fires:: PropertyAggregator::TAggregation aggType = fires::PropertyAggregator::SUM;
    switch ( op )
    {
    case TAutoUpdatePropertyOp::SUM:
      break;
    case TAutoUpdatePropertyOp::MEAN:
      aggType = fires::PropertyAggregator::MEAN;
      break;
    case TAutoUpdatePropertyOp::MAX:
      aggType = fires::PropertyAggregator::MAX;
      break;
    case TAutoUpdatePropertyOp::MIN:
      aggType = fires::PropertyAggregator::MIN;
      break;
    default:
      assert( false );
    }

    aggregateConfig.addProperty( origPropertyLabel, aggregator, aggType );

    aggregate.eval( objs, aggregateConfig );
    assert( objs.size( ) == 1 );
    this->setProperty( destPropertyLabel,
      objs.front( )->getProperty( origPropertyLabel ));
  }

}
