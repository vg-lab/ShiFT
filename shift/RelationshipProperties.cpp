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
#include <shift/shift.h>
#include "RelationshipProperties.h"

namespace shift
{
  void RelationshipProperties::autoUpdatePropertyWithRelatedRelations(
      const std::vector <std::string>& relatedEntitiesNames_,
      RelationshipProperties::TAutoUpdatePropertyOp op_,
      const std::string& origPropertyLabel_,
      const std::string& destPropertyLabel_ )
  {
    if ( !_relatedRelations )
    {
      return;
    }
    fires::Objects objs;
    for( auto relRelation : *_relatedRelations )
    {
      for( const auto& relatedEntitiesName : relatedEntitiesNames_ )
        if( relRelation->relationName( ) == relatedEntitiesName )
          if( relRelation->hasProperty( origPropertyLabel_ ) ||
              op_ == TAutoUpdatePropertyOp::COUNT )
            objs.add( relRelation );
    }

    if( op_ == TAutoUpdatePropertyOp::COUNT )
    {
      this->setProperty( destPropertyLabel_, uint( objs.size( ) ) );
      return;
    }

    auto aggregator = fires::PropertyManager::getAggregator(
        origPropertyLabel_ );
    fires::Aggregate aggregate;
    fires::AggregateConfig aggregateConfig;
    fires::PropertyAggregator::TAggregation aggType =
        fires::PropertyAggregator::SUM;
    switch( op_ )
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
        SHIFT_THROW( "Unknown aggregated operation type." );
    }

    aggregateConfig.addProperty( origPropertyLabel_, aggregator, aggType );
    aggregate.eval( objs, aggregateConfig );
    //SHIFT_CHECK_THROW( objs.size( ) == 1, "Objects size must be 1." );
    this->setProperty( destPropertyLabel_,
      objs.front( )->getProperty( origPropertyLabel_ ));

  }

  void RelationshipProperties::relatedRelations(
      std::vector< RelationshipProperties* >* relatedRelations_ )
  {
    _relatedRelations = relatedRelations_;
  }

  bool RelationshipProperties::hasPropertyFlag( const std::string&,
    RelationshipProperties::TPropertyFlag ) const
  {
    return false;
  }
}