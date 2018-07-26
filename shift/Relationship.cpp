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
#include <shift/shift.h>
#include "Relationship.h"

namespace shift
{

  Relationship::Relationship( const std::string& name_ )
    : _cardinality( UNDEFINED )
    , _name( name_ )
  {
  }

  Relationship::TCardinality Relationship::cardinality( void ) const
  {
    return _cardinality;
  }
  RelationshipAggregatedOneToN* Relationship::asAggregatedOneToN( )
  {
    return nullptr;
  }
  RelationshipOneToOne* Relationship::asOneToOne( void )
  {
    return nullptr;
  }
  RelationshipOneToN* Relationship::asOneToN( void )
  {
    return nullptr;
  }
  RelationshipNToN* Relationship::asNToN( void )
  {
    return nullptr;
  }

  RelationshipOneToOne::RelationshipOneToOne( const std::string& name_ )
    : Relationship( name_ )
  {
    _cardinality = ONE_TO_ONE;
  }

  RelationshipOneToOne* RelationshipOneToOne::asOneToOne( void )
  {
    return this;
  }

  RelationshipOneToN::RelationshipOneToN( const std::string& name_ )
    : Relationship( name_ )
  {
    _cardinality = ONE_TO_N;
  }

  RelationshipOneToN* RelationshipOneToN::asOneToN( void )
  {
    return this;
  }

  RelationshipNToN::RelationshipNToN( const std::string& name_ )
    : Relationship( name_ )
  {
    _cardinality = N_TO_N;
  }

  RelationshipNToN* RelationshipNToN::asNToN( void )
  {
    return this;
  }

  void Relationship::Establish( RelationshipOneToN& relOneToN,
    RelationshipOneToOne& relOneToOne,
    Entity* entityOrig, Entity* entityDest )
  {
    SHIFT_CHECK_THROW( entityOrig && entityDest, "Entity doesnt exist" );
    auto entityOrigGid = entityOrig->entityGid( );
    auto entityDestGid = entityDest->entityGid( );
    relOneToN[ entityOrigGid ].insert( RelationshipOneToNDest( entityDestGid,
                                                               nullptr ));
    relOneToOne[ entityDestGid ].entity = entityOrigGid;

    entityOrig->setRelatedDependencies( relOneToN.name( ), entityDest );
    entityDest->setRelatedDependencies( relOneToOne.name( ), entityOrig );

  }

  void Relationship::Establish( RelationshipOneToN& relOneToNOrig,
    RelationshipOneToN& relOneToNDest, Entity* entityOrig,
    Entity* entityDest, RelationshipProperties* propertiesOrig,
    RelationshipProperties* propertiesDest  )
  {
    SHIFT_CHECK_THROW( entityOrig && entityDest, "Entity doesnt exist" );
    auto entityOrigGid = entityOrig->entityGid( );
    auto entityDestGid = entityDest->entityGid( );

    relOneToNOrig[ entityOrigGid ].insert(
      RelationshipOneToNDest( entityDestGid, propertiesOrig ));
    relOneToNDest[ entityDestGid ].insert(
      RelationshipOneToNDest( entityOrigGid, propertiesDest ));
  }


  void Relationship::Establish( RelationshipAggregatedOneToN& relOneToNOrig,
    RelationshipAggregatedOneToN& relOneToNDest, Entity* entityOrig,
    Entity* entityDest, Entity* entityBaseOrig, Entity* entityBaseDest,
    RelationshipProperties* propertiesOrig,
    RelationshipProperties* propertiesDest )
  {
    SHIFT_CHECK_THROW( entityOrig && entityDest && entityBaseOrig
      && entityBaseDest, "Entity doesnt exist" );
    auto entityOrigGid = entityOrig->entityGid( );
    auto entityDestGid = entityDest->entityGid( );
    auto entityBaseOrigGid = entityBaseOrig->entityGid( );
    auto entityBaseDestGid = entityBaseDest->entityGid( );
    SHIFT_CHECK_THROW( entityOrigGid != entityBaseOrigGid
      || entityBaseDestGid != entityDestGid, "Aggregating normal connection" );

    relOneToNOrig.addBaseRelation( entityOrigGid, entityDestGid,
      entityBaseOrigGid, entityBaseDestGid, propertiesOrig );
    relOneToNDest.addBaseRelation( entityDestGid, entityOrigGid,
      entityBaseDestGid, entityBaseOrigGid, propertiesDest );
  }

  void Relationship::EstablishWithHierarchy( RelationshipOneToN& relOneToN,
    RelationshipOneToOne& relOneToOne,
    RelationshipAggregatedOneToN& /*relAggregatedOneToNOrig*/,
    RelationshipAggregatedOneToN& /*relAggregatedOneToNDest*/,
    Entity* entityOrig, Entity* entityDest )
  {
    Establish(relOneToN,relOneToOne,entityOrig,entityDest);
    //todo: modify aggregated dependencies

    //RelationshipOneToN& relOriginalOneToNOrig =
    // *(relAggregatedOneToNOrig._baseRelationShip);
    //RelationshipOneToN& relOriginalOneToNDest =
    // *(relAggregatedOneToNDest._baseRelationShip);
    //RelationshipOneToOne& relSearchOneToOne =
    // *(relAggregatedOneToNOrig._hierarchyRelationShip);

    //Search entity and look for all the aggregated relationships
    //Add to all the aggregatedRelations the new parent
    //Repeat for connections
  }


  void Relationship::EstablishAndAggregate(
    RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
    RelationshipAggregatedOneToN& relAggregatedOneToNDest,
    Entities& searchEntities, Entity* entityOrig,
    Entity* entityDest, RelationshipProperties* propertiesOrig,
    RelationshipProperties* propertiesDest )
  {
    RelationshipOneToN& relOriginalOneToNOrig =
      *(relAggregatedOneToNOrig._baseRelationShip);
    RelationshipOneToN& relOriginalOneToNDest =
      *(relAggregatedOneToNDest._baseRelationShip);
    RelationshipOneToOne& relHierarchyOneToOne =
      *(relAggregatedOneToNOrig._hierarchyRelationShip);

    Establish( relOriginalOneToNOrig, relOriginalOneToNDest, entityOrig,
      entityDest, propertiesOrig, propertiesDest );
    if ( entityDest->entityGid( ) != entityOrig->entityGid( ))
    {
      Entities* entitiesDest = new Entities( );
      entitiesDest->addRelatedEntitiesOneToOne( relHierarchyOneToOne,
        entityDest, searchEntities );

      for ( Entity* relatedDest : entitiesDest->vector( ))
      {
        Establish( relAggregatedOneToNOrig, relAggregatedOneToNDest, entityOrig,
          relatedDest, entityOrig, entityDest, propertiesOrig, propertiesDest );
      }

      Entities* entitiesOrig = new Entities( );
      entitiesDest->add( entityDest );
      entitiesOrig->addRelatedEntitiesOneToOne( relHierarchyOneToOne,
        entityOrig, searchEntities, 0, entitiesDest );

      for ( Entity* relatedOrig : entitiesOrig->vector( ))
      {
        for ( Entity* relatedDest : entitiesDest->vector( ))
        {
          Establish( relAggregatedOneToNOrig, relAggregatedOneToNDest,
            relatedOrig, relatedDest, entityOrig, entityDest, propertiesOrig,
            propertiesDest );
        }
      }
    }
  }

  bool RelationshipAggregatedOneToN::removeDependentRelation(
    EntityGid /*entityToSearch*/, EntityGid /*entityOrigRemove*/,
    EntityGid /*entityDestRemove*/ )
  {
    //todo
    return false;
  }

  bool RelationshipAggregatedOneToN::removeDependentRelation(
    EntityGid /*entityToSearch*/, EntityGid /*entityOrigRemove*/ )
  {
    //todo
    return false;
  }

  void RelationshipAggregatedOneToN::addBaseRelation( EntityGid entityOrig,
    EntityGid entityDest, EntityGid entityBaseOrig, EntityGid entityBaseDest,
    RelationshipProperties* relationshipBaseProperties )
  {
    const auto mapRelationsOrigIt = mapRelations->find( entityOrig );
    RelationshipProperties* relationshipProperties = nullptr;
    RelationshipAggregatedOneToNProperties*
      relationshipAggregatedOneToNProperties = nullptr;
    std::vector < RelationshipProperties* >* vectorProperties;
    if ( mapRelationsOrigIt != mapRelations->end( ))
    {
      std::map < EntityGid, RelationshipAggregatedOneToNProperties >*
        secondMap = mapRelationsOrigIt->second;
      const auto mapRelationsDestIt = secondMap->find( entityDest );
      if ( mapRelationsDestIt != secondMap->end( ))
      {
        relationshipAggregatedOneToNProperties = &mapRelationsDestIt->second;
        relationshipProperties =
          mapRelationsDestIt->second.relationshipAggregatedProperties;
        vectorProperties = mapRelationsDestIt->second.basedProperties;
        if ( relationshipBaseProperties )
        {
          auto insertIt =
            std::find( vectorProperties->begin( ), vectorProperties->end( ),
            relationshipBaseProperties );
          SHIFT_CHECK_THROW( insertIt == vectorProperties->end( ),
            "Repeated not null relation properties" );
        }
      }
      else
      {
        vectorProperties = new std::vector < RelationshipProperties* >( );
        relationshipProperties = _baseProperties->create( );
        relationshipAggregatedOneToNProperties =
          new RelationshipAggregatedOneToNProperties( vectorProperties,
          relationshipProperties );
        secondMap->insert( std::make_pair( entityDest,
          *relationshipAggregatedOneToNProperties ));
      }
    }
    else
    {
      const auto secondMap =
        new std::map < EntityGid, RelationshipAggregatedOneToNProperties >( );

      vectorProperties = new std::vector < RelationshipProperties* >( );
      relationshipProperties = _baseProperties->create( );
      relationshipAggregatedOneToNProperties =
        new RelationshipAggregatedOneToNProperties( vectorProperties,
        relationshipProperties );
      secondMap->insert(
        std::make_pair( entityDest, *relationshipAggregatedOneToNProperties ));

      mapRelations->insert( std::make_pair( entityOrig, secondMap ));

    }
    vectorProperties->push_back( relationshipBaseProperties );


    auto mapBaseOrigIt = mapBase->find( entityBaseOrig );
    std::vector< std::pair
      < EntityGid, RelationshipAggregatedOneToNProperties >>* vectorEntities;
    auto pair = std::pair < EntityGid, RelationshipAggregatedOneToNProperties >(
      entityDest, *relationshipAggregatedOneToNProperties );

    if ( mapBaseOrigIt != mapBase->end( ))
    {
      const std::map < EntityGid, RelationshipBasesAggregatedOneToN >
        * secondMap = mapBaseOrigIt->second;
      const auto mapBaseDestIt = secondMap->find( entityBaseDest );

      if ( mapBaseDestIt != secondMap->end( ))
      {
        SHIFT_CHECK_THROW( mapBaseDestIt->second.relationProperties ==
          relationshipBaseProperties,
          "Different properties for the same connection" );
        std::map < EntityGid, std::vector <
          std::pair< EntityGid, RelationshipAggregatedOneToNProperties >>* >
          * mapEntities = mapBaseDestIt->second.aggregatedRelations;
        auto mapEntitiesIt = mapEntities->find( entityOrig );
        if ( mapEntitiesIt != mapEntities->end( ))
        {
          vectorEntities = mapEntitiesIt->second;
//          auto insertIt =
//            std::find( vectorEntities->begin( ), vectorEntities->end( ),
//                       pair );
//          SHIFT_CHECK_THROW( insertIt == vectorEntities->end( ),
//                             "Repeated aggregation of a connection" );
        }
        else
        {
          vectorEntities = new std::vector <
            std::pair< EntityGid, RelationshipAggregatedOneToNProperties >>( );
        }
      }
      else
      {
        vectorEntities = new std::vector <
          std::pair< EntityGid, RelationshipAggregatedOneToNProperties >>( );
      }
    }
    else
    {
      vectorEntities = new std::vector <
        std::pair< EntityGid, RelationshipAggregatedOneToNProperties >>( );
      auto mapEntities = new std::map < EntityGid, std::vector <
        std::pair< EntityGid, RelationshipAggregatedOneToNProperties >>* >( );
      mapEntities->insert( std::make_pair( entityBaseDest, vectorEntities ));
      RelationshipBasesAggregatedOneToN* baseProperties =
        new RelationshipBasesAggregatedOneToN( mapEntities,
        relationshipBaseProperties );
      auto secondMap =
        new std::map< EntityGid, RelationshipBasesAggregatedOneToN >( );
      secondMap->insert( std::make_pair( entityDest, *baseProperties ));
      mapBase->insert( std::make_pair( entityBaseOrig, secondMap ));
    }
    vectorEntities->push_back( pair );

    if ( relationshipProperties )
    {
      //todo: aggregated properties in relationships
      if ( relationshipProperties->hasProperty( "Weight" ))
      {
        relationshipProperties->getProperty( "Weight" )
          .set( float( 1.0f + vectorProperties->size( )));
      }
      else if ( relationshipProperties->hasProperty( "count" ))
      {
        relationshipProperties->getProperty( "count" )
          .set( vectorProperties->size( ));
      }
      else
      {
        SHIFT_THROW( "Expected weight or count" )
      }
    }
  }



  RelationshipAggregatedOneToN*
  RelationshipAggregatedOneToN::asAggregatedOneToN( void )
  {
    return this;
  }

  RelationshipAggregatedOneToN::RelationshipAggregatedOneToN(
    const std::string& name_,
    RelationshipProperties* baseProperties_,
    RelationshipOneToOne* hierarchyRelationShip_,
    RelationshipOneToN* baseRelationShip_ )
    : Relationship( name_ )
    , _baseProperties( baseProperties_ )
    , _hierarchyRelationShip( hierarchyRelationShip_ )
    , _baseRelationShip( baseRelationShip_ )

  {
    _cardinality = ONE_TO_N;
    mapRelations = new std::map< EntityGid,
      std::map< EntityGid, RelationshipAggregatedOneToNProperties >*>( );
    mapBase = new std::map< EntityGid,
      std::map< EntityGid, RelationshipBasesAggregatedOneToN >*>( );

  }
} // namespace shift
