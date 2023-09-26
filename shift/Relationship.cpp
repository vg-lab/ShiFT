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
#include "Relationship.h"
#include <shift/shift.h>

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

  RelationshipProperties*
  RelationshipOneToN::getRelationProperties( EntityGid entityOrig,
    EntityGid entityDest )
  {
    auto relationIt = this->find( entityOrig );
    if( relationIt != this->end( ))
    {
      auto relationMMIt =
        relationIt->second.find( entityDest );
      if( relationMMIt != relationIt->second.end( ))
      {
        return relationMMIt->second;
      }
    }
    return nullptr;
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

  void Relationship::clearRelations( void )
  {
    std::cout << "Use type implementation.";
  }

  void RelationshipNToN::clearRelations( void )
  {
    this->clear( );
  }

  void RelationshipOneToN::clearRelations( void )
  {
    this->clear( );
  }

  void RelationshipOneToOne::clearRelations( void )
  {
    this->clear( );
  }

  void RelationshipAggregatedOneToN::clearRelations( void )
  {
    _mapBaseRels.clear( );
    _mapAggregatedRels.clear( );
  }

  void Relationship::Establish( RelationshipOneToN& relOneToN,
    RelationshipOneToOne& relOneToOne,
    Entity* entityOrig, Entity* entityDest )
  {
    //Checks the entity exists
    SHIFT_CHECK_THROW( entityOrig && entityDest, "Entity doesnt exist" );
    //Obtain all the entites GIDs
    const auto entityOrigGid = entityOrig->entityGid( );
    const auto entityDestGid = entityDest->entityGid( );
    //Inserts the relation
    relOneToN[ entityOrigGid ].insert( RelationshipOneToNDest( entityDestGid,
      nullptr ));
    relOneToOne[ entityDestGid ].entity = entityOrigGid;
    //insert entities dependencies
    entityOrig->setRelatedDependencies( relOneToN.name( ), entityDest );
    entityDest->setRelatedDependencies( relOneToOne.name( ), entityOrig );
  }

  void Relationship::relationBreak( RelationshipOneToN& relOneToNOrig,
    RelationshipOneToN& relOneToNDest, Entity* entityOrig,
    Entity* entityDest )
  {
    //Checks the entity exists
    SHIFT_CHECK_THROW( entityOrig && entityDest, "Entity doesnt exist" );
    //Obtain all the entites GIDs
    const auto entityOrigGid = entityOrig->entityGid( );
    const auto entityDestGid = entityDest->entityGid( );
    //Finds and erase the orig-dest origRelation
    auto entityRelations = relOneToNOrig.find( entityOrigGid );
    SHIFT_CHECK_THROW( entityRelations != relOneToNOrig.end( ),
      "Original relation doesn't exist" );
    auto connectionIt = entityRelations->second.find( entityDestGid );
    RelationshipProperties* origProperties = nullptr;
    if( connectionIt != entityRelations->second.end( ))
    {
      origProperties = connectionIt->second;
      delete origProperties;
      if(entityRelations->second.size( ) == 1)
      {
        relOneToNOrig.erase( entityRelations );
      }
      else
      {
        entityRelations->second.erase( connectionIt );
      }
    }

    //Finds and erase the dest-orig destRelation
    entityRelations = relOneToNDest.find( entityDestGid );
    SHIFT_CHECK_THROW( entityRelations != relOneToNDest.end( ),
      "Destiny relation doesn't exist" );
    connectionIt = entityRelations->second.find( entityOrigGid );
    if( connectionIt != entityRelations->second.end( ))
    {
      if( connectionIt->second && connectionIt->second != origProperties )
      {
        delete connectionIt->second;
      }
      if( entityRelations->second.size( ) == 1 )
      {
        relOneToNDest.erase( entityRelations );
      }
      else
      {
        entityRelations->second.erase( connectionIt );
      }
    }
  }

  void Relationship::relationBreak( RelationshipOneToN& relOneToNOrig,
    RelationshipOneToOne& relOneToOneDest, Entity* entityOrig,
    Entity* entityDest )
  {
    //Checks the entity exists
    SHIFT_CHECK_THROW( entityOrig && entityDest, "Entity doesnt exist" );
    //Obtain all the entites GIDs
    const auto entityOrigGid = entityOrig->entityGid( );
    const auto entityDestGid = entityDest->entityGid( );
    //Finds and erase the orig-dest origRelation
    auto entityRelations = relOneToNOrig.find( entityOrigGid );
    SHIFT_CHECK_THROW( entityRelations != relOneToNOrig.end( ),
                       "Original relation doesn't exist" );
    auto connectionIt = entityRelations->second.find( entityDestGid );
    if( connectionIt != entityRelations->second.end( ))
    {
      if(entityRelations->second.size( ) == 1)
      {
        relOneToNOrig.erase( entityRelations );
      }
      else
      {
        entityRelations->second.erase( connectionIt );
      }
    }

    //Finds and erase the dest-orig destRelation
    auto entityRelations1 = relOneToOneDest.find( entityDestGid );
    SHIFT_CHECK_THROW( entityRelations1 != relOneToOneDest.end( ),
      "Destiny relation doesn't exist" );
    relOneToOneDest.erase( entityRelations1 );

    //insert entities dependencies
    entityOrig->removeRelatedDependencies( relOneToNOrig.name( ), entityDest );
    entityDest->removeRelatedDependencies( relOneToOneDest.name( ), entityOrig );

  }


  void Relationship::Establish( RelationshipOneToN& relOneToNOrig,
    RelationshipOneToN& relOneToNDest, Entity* entityOrig,
    Entity* entityDest, RelationshipProperties* propertiesOrig,
    RelationshipProperties* propertiesDest  )
  {
    //Checks the entity exists
    SHIFT_CHECK_THROW( entityOrig && entityDest, "Entity doesn't exist" );
    //Obtain all the entites GIDs
    const auto entityOrigGid = entityOrig->entityGid( );
    const auto entityDestGid = entityDest->entityGid( );
    //Inserts the relation
    relOneToNOrig[ entityOrigGid ].insert(
      RelationshipOneToNDest( entityDestGid, propertiesOrig ));
    relOneToNDest[ entityDestGid ].insert(
      RelationshipOneToNDest( entityOrigGid, propertiesDest ));
  }

  void Relationship::Establish( RelationshipAggregatedOneToN& relOneToNOrig,
    RelationshipAggregatedOneToN& relOneToNDest, Entity* entityOrig,
    Entity* entityDest, Entity* entityBaseOrig, Entity* entityBaseDest,
    RelationshipProperties* propertiesOrig,
    RelationshipProperties* propertiesDest,
    const bool recalcProperties )
  {
    //Checks the entity exists
    SHIFT_CHECK_THROW( entityOrig && entityDest && entityBaseOrig
      && entityBaseDest, "Entity doesnt exist" );
    //Obtain all the entites GIDs
    const auto entityOrigGid = entityOrig->entityGid( );
    const auto entityDestGid = entityDest->entityGid( );
    const auto entityBaseOrigGid = entityBaseOrig->entityGid( );
    const auto entityBaseDestGid = entityBaseDest->entityGid( );
    //Checks that the aggregated relation it's not the original relation
    SHIFT_CHECK_THROW( entityOrigGid != entityBaseOrigGid
      || entityBaseDestGid != entityDestGid, "Aggregating normal connection" );
    const std::string entityOrigName = entityOrig->
      getPropertyValue<std::string>( "Entity name", " " );
    const std::string entityDestName = entityDest->
        getPropertyValue<std::string>( "Entity name", " " );
    //Inserts the aggregated relation
    relOneToNOrig.addBaseRelation( entityOrigGid, entityDestGid,
      entityBaseOrigGid, entityBaseDestGid, propertiesOrig, recalcProperties,
      entityOrigName, entityDestName);
    relOneToNDest.addBaseRelation( entityDestGid, entityOrigGid,
      entityBaseDestGid, entityBaseOrigGid, propertiesDest,recalcProperties,
      entityDestName, entityOrigName );
  }

  void Relationship::BreakAnAggregatedRelation(
    RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
    RelationshipAggregatedOneToN& relAggregatedOneToNDest,
    Entity* entityOrig, Entity* entityDest )
  {
    //Get base relations
    RelationshipOneToN& relOriginalOneToNOrig =
      *( relAggregatedOneToNOrig.baseRelationShip( ));
    RelationshipOneToN& relOriginalOneToNDest =
      *( relAggregatedOneToNDest.baseRelationShip( ));

    //Obtain entities Gids
    const auto destGid = entityDest->entityGid( );
    const auto origGid = entityOrig->entityGid( );

    //Checks that it's not a auto-relation
    if ( destGid != origGid )
    {
      //Update the dependent aggregated relations
      relAggregatedOneToNDest.removeDependentRelation( destGid, origGid );
      relAggregatedOneToNOrig.removeDependentRelation( origGid, destGid );
    }

    //Break the original relation
    relationBreak( relOriginalOneToNOrig, relOriginalOneToNDest, entityOrig,
      entityDest );
  }

  void Relationship::EstablishAndAggregate(
    RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
    RelationshipAggregatedOneToN& relAggregatedOneToNDest,
    const Entities& searchEntities, Entity* entityOrig,
    Entity* entityDest, RelationshipProperties* propertiesOrig,
    RelationshipProperties* propertiesDest, bool recalcProperties  )
  {
    RelationshipOneToN& relOriginalOneToNOrig =
      *(relAggregatedOneToNOrig.baseRelationShip( ));
    RelationshipOneToN& relOriginalOneToNDest =
      *(relAggregatedOneToNDest.baseRelationShip( ));
    RelationshipOneToOne& relHierarchyOneToOne =
      *(relAggregatedOneToNOrig.hierarchyRelationShip( ));

    //Establish original relation
    Establish( relOriginalOneToNOrig, relOriginalOneToNDest, entityOrig,
      entityDest, propertiesOrig, propertiesDest );

    AggregateDependentRelations( relAggregatedOneToNOrig,
      relAggregatedOneToNDest, searchEntities, entityOrig, entityDest,
      propertiesOrig, propertiesDest, recalcProperties, relHierarchyOneToOne );
  }

  void Relationship::AggregateDependentRelations(
    RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
    RelationshipAggregatedOneToN& relAggregatedOneToNDest,
    const Entities& searchEntities, Entity* entityOrig,
    Entity* entityDest, RelationshipProperties* propertiesOrig,
    RelationshipProperties* propertiesDest, bool recalcProperties,
    const RelationshipOneToOne& relHierarchyOneToOne )
  {
    if ( entityDest->entityGid( ) != entityOrig->entityGid( ))
    {
      //Search for all not common parents
      Entities entitiesDest;
      entitiesDest.addRelatedEntitiesOneToOne( relHierarchyOneToOne,
        entityDest, searchEntities );
      Entities entitiesOrig;
      entitiesDest.add( entityDest );
      entitiesOrig.addRelatedEntitiesOneToOne( relHierarchyOneToOne,
        entityOrig, searchEntities, 0, &entitiesDest );
      entitiesDest.remove( entityDest );

      //Establishes between origin whith all entities related with the destiny
      for ( Entity* relatedDest : entitiesDest.vector( ))
      {
        Establish( relAggregatedOneToNOrig, relAggregatedOneToNDest, entityOrig,
          relatedDest, entityOrig, entityDest, propertiesOrig, propertiesDest,
          recalcProperties );
      }
      //Establishes between all origin relatives with the
      //destiny and all their relatives
      entitiesDest.add( entityDest );
      for ( Entity* relatedOrig : entitiesOrig.vector( ))
      {
        for ( Entity* relatedDest : entitiesDest.vector( ))
        {
          Establish( relAggregatedOneToNOrig, relAggregatedOneToNDest,
            relatedOrig, relatedDest, entityOrig, entityDest, propertiesOrig,
            propertiesDest, recalcProperties );
        }
      }
    }
  }

  void RelationshipAggregatedOneToN::removeDependentRelation(
    EntityGid entityOrigRemove, EntityGid entityDestRemove )
  {
    //Search in the base map the original relation origin
    const auto baseOrigIt = _mapBaseRels.find( entityOrigRemove );
    if ( baseOrigIt == _mapBaseRels.end( ))
    {
      //If not aggregated relations are found for this entity, return
      return;
    }

    //Find in the original relation origin-destiny
    const auto baseRelationIt = baseOrigIt->second->find( entityDestRemove );
    if ( baseRelationIt == baseOrigIt->second->end( ))
    {
      //If not aggregated relations are found for this relation, return
      return;
    }
    const auto aggregatedDependencies =
      baseRelationIt->second.aggregatedRelations;
    auto* relationProperties = baseRelationIt->second.relationProperties;
    //Iterates over all the aggregated relations origin of the base relation
    for ( auto dependentOrigIt = aggregatedDependencies->begin( );
      dependentOrigIt != aggregatedDependencies->end( ); dependentOrigIt++ )
    {
      auto AgreggatedOrigIt =
        _mapAggregatedRels.find( dependentOrigIt->first );
      //Iterates over all the aggregated relations of the base relation
      for ( auto dependentRelIt = dependentOrigIt->second->begin( );
        dependentRelIt != dependentOrigIt->second->end( ); dependentRelIt++ )
      {
        //Remove the base relation properties from the list of relation
        //properties use to calculate the aggregated relation properties
        auto baseProperties = dependentRelIt->second.basedProperties;
        auto removePropertyPosition = std::find( baseProperties->begin( ),
          baseProperties->end( ), relationProperties );
        SHIFT_CHECK_THROW( removePropertyPosition != baseProperties->end( ),
          "Error base properties are not in the dependent properties" );
        baseProperties->erase( removePropertyPosition );

        if( dependentRelIt->second.basedProperties->empty( ))
        {
          //If the list of relation properties its empty,
          //erase the aggregated relation
          AgreggatedOrigIt->second->erase(
            AgreggatedOrigIt->second->find( dependentRelIt->first ));
        }
        else
        {
          //Update the relation properties to reflect
          //the new list of relation properties
          auto relationAggregatedProperties =
            dependentRelIt->second.relationshipAggregatedProperties;
          //relationAggregatedProperties->
          //  removeRelatedDependencies( relationProperties );
          //relationProperties->removeRelatedDependencies(
          //  relationAggregatedProperties.get( ));
          relationAggregatedProperties->autoUpdateProperties( );
        }
      }
      if ( AgreggatedOrigIt->second->empty( ))
      {
        //If the origin has not aggregated relations left,
        //remove the origin from the map
        _mapAggregatedRels.erase( AgreggatedOrigIt );
      }

    }
    //Memory free
    baseOrigIt->second->erase( baseRelationIt );
    if(baseOrigIt->second->empty( ))
    {
      _mapBaseRels.erase( baseOrigIt );
    }
  }

  void RelationshipAggregatedOneToN::addBaseRelation( EntityGid entityOrig,
    EntityGid entityDest, EntityGid entityBaseOrig, EntityGid entityBaseDest,
    RelationshipProperties* relationshipBaseProperties,
    const bool recalcProperties, const std::string& entityOrigName,
    const std::string& entityDestName )
  {
    //Update/creates the aggregated relation

    //Find the aggregated relation origin
    const auto aggregatedOrigIt = _mapAggregatedRels.find( entityOrig );

    std::shared_ptr< RelationshipProperties >
      aggregatedRelationProperties;
    RelationshipAggregatedOneToNProperties*
      relationshipAggregatedOneToNProperties;
    std::shared_ptr< std::vector < RelationshipProperties* >>
      baseVectorProperties;

    if ( aggregatedOrigIt != _mapAggregatedRels.end( ))
    {
      //The aggregated origin has already aggregated relations
      std::shared_ptr< AggregatedOneToNAggregatedDests >
        aggregatedDestsMap( aggregatedOrigIt->second );
      //Find the aggregated relation
      const auto aggregatedRelIt = aggregatedDestsMap->find( entityDest );
      if ( aggregatedRelIt != aggregatedDestsMap->end( ))
      {
        //The aggregated relation already exists so its updated
        //Stores the existing relation properties
        relationshipAggregatedOneToNProperties = &aggregatedRelIt->second;
        aggregatedRelationProperties =
          aggregatedRelIt->second.relationshipAggregatedProperties;
        baseVectorProperties = aggregatedRelIt->second.basedProperties;
      }
      else
      {
        //The aggregated relation doesnt exit,
        //Their properties are created and stored
        baseVectorProperties.reset(
          new std::vector < RelationshipProperties* >( ));
        aggregatedRelationProperties.reset(
          _baseAggregatedProperties->create( ));
        aggregatedRelationProperties->setProperty( "Name",
          std::string( "AR:" + entityOrigName + "-"
          + entityDestName ));
        aggregatedRelationProperties->relatedRelations(
          baseVectorProperties.get( ));
        relationshipAggregatedOneToNProperties =
          new RelationshipAggregatedOneToNProperties( baseVectorProperties,
          aggregatedRelationProperties );
        //The new aggregated relation its added to the map
        aggregatedDestsMap->insert( std::make_pair( entityDest,
          *relationshipAggregatedOneToNProperties ));
      }
    }
    else
    {
      //The aggregated origin its not related
      //The aggregated relation properties are created and stored
      baseVectorProperties.reset(
        new std::vector < RelationshipProperties* >( ));
      aggregatedRelationProperties.reset( _baseAggregatedProperties->create( ));
      aggregatedRelationProperties->setProperty( "Name",
        std::string( "AR:"+entityOrigName+"-"+entityDestName ));
      aggregatedRelationProperties->relatedRelations(
        baseVectorProperties.get( ));
      relationshipAggregatedOneToNProperties =
        new RelationshipAggregatedOneToNProperties( baseVectorProperties,
        aggregatedRelationProperties );

      //The new aggregated relation its added to the map,
      const std::shared_ptr< AggregatedOneToNAggregatedDests >
        aggregatedDestsMap( new AggregatedOneToNAggregatedDests( ));
      aggregatedDestsMap->insert( std::make_pair(
        entityDest, *relationshipAggregatedOneToNProperties ));
      _mapAggregatedRels.insert( std::make_pair( entityOrig,
        aggregatedDestsMap ));
    }
    //The properties of the base relation are added to the aggregated relation
    baseVectorProperties->push_back( relationshipBaseProperties );

    //Update/Adds the base reference relations

    //Find the base relation origin
    auto mapBaseOrigIt = _mapBaseRels.find( entityBaseOrig );

    std::shared_ptr< AggregatedOneToNDependentProperties > dependentDestsVector;
    auto pair = AggregatedOneToNDependentProperty(
      entityDest, *relationshipAggregatedOneToNProperties );

    if ( mapBaseOrigIt != _mapBaseRels.end( ))
    {
      //The base origin has already relations
      std::shared_ptr< AggregatedOneToNBaseDests >
        baseDestsMap( mapBaseOrigIt->second );
      //Find the base relation
      const auto baseRelIt = baseDestsMap->find( entityBaseDest );
      if ( baseRelIt != baseDestsMap->end( ))
      {
        //The base relation already exists so its updated
        //Stores the existing relation properties
        baseRelIt->second.relationProperties = relationshipBaseProperties;
        std::shared_ptr< AggregatedOneToNDependentRelations >
          dependentOriginsMap = baseRelIt->second.aggregatedRelations;
        auto dependentOrigIt = dependentOriginsMap->find( entityOrig );
        if ( dependentOrigIt != dependentOriginsMap->end( ))
        {
          //The aggregated origin already depends of the relation
          dependentDestsVector = dependentOrigIt->second;
        }
        else
        {
          //Creates and insert a new dependency between origin and destiny
          dependentDestsVector.reset(
            new AggregatedOneToNDependentProperties( ));
          dependentOriginsMap->insert(
            std::make_pair( entityOrig, dependentDestsVector ));
        }
      }
      else
      {
        //The base relation doesnt exit,
        //Their properties are created and stored
        dependentDestsVector.reset(
          new AggregatedOneToNDependentProperties( ));
        std::shared_ptr< AggregatedOneToNDependentRelations >
          dependentOriginsMap( new AggregatedOneToNDependentRelations( ));
        dependentOriginsMap->insert(
          std::make_pair( entityOrig, dependentDestsVector ));
        RelationshipBasesAggregatedOneToN* baseProperties =
          new RelationshipBasesAggregatedOneToN( dependentOriginsMap,
          relationshipBaseProperties );
        //The new base relation its added to the map
        baseDestsMap->insert(
          std::make_pair( entityBaseDest, *baseProperties ));

      }
    }
    else
    {
      //The base origin its not related
      //The base relation properties are created and stored
      dependentDestsVector.reset( new AggregatedOneToNDependentProperties( ));
      std::shared_ptr< AggregatedOneToNDependentRelations >
          dependentOriginsMap( new AggregatedOneToNDependentRelations( ));
      dependentOriginsMap->insert(
        std::make_pair( entityOrig, dependentDestsVector ));
      RelationshipBasesAggregatedOneToN* baseProperties =
        new RelationshipBasesAggregatedOneToN( dependentOriginsMap,
        relationshipBaseProperties );
      std::shared_ptr< AggregatedOneToNBaseDests > baseDestsMap(
        new AggregatedOneToNBaseDests( ));
      baseDestsMap->insert(
        std::make_pair( entityBaseDest, *baseProperties ));
      //The new base relation its added to the map
      _mapBaseRels.insert( std::make_pair( entityBaseOrig, baseDestsMap ));
    }

    dependentDestsVector->push_back( pair );
    //aggregatedRelationProperties->setRelatedDependencies( relationshipBaseProperties);
    //relationshipBaseProperties->setRelatedDependencies( aggregatedRelationProperties.get( ));
    if( recalcProperties )
    {
      aggregatedRelationProperties->autoUpdateProperties( );
    }
  }

  RelationshipAggregatedOneToN*
  RelationshipAggregatedOneToN::asAggregatedOneToN( void )
  {
    return this;
  }

  RelationshipAggregatedOneToN::RelationshipAggregatedOneToN(
    const std::string& name_,
    RelationshipProperties* baseAggregatedProperties_,
    RelationshipOneToOne* hierarchyRelationShip_,
    RelationshipOneToN* baseRelationShip_ )
    : Relationship( name_ )
    , _baseAggregatedProperties( baseAggregatedProperties_ )
    , _hierarchyRelationShip( hierarchyRelationShip_ )
    , _baseRelationShip( baseRelationShip_ )
  {
    _cardinality = ONE_TO_N;
  }

  RelationshipOneToOne*
  RelationshipAggregatedOneToN::hierarchyRelationShip( ) const
  {
    return _hierarchyRelationShip;
  }

  RelationshipOneToN*
  RelationshipAggregatedOneToN::baseRelationShip( ) const
  {
    return _baseRelationShip;
  }

  AggregatedOneToNAggregatedRels&
  RelationshipAggregatedOneToN::mapAggregatedRels( )
  {
    return _mapAggregatedRels;
  }

  void
  RelationshipAggregatedOneToN::updateDependentRelations( EntityGid origGid,
    EntityGid destGid )
  {
    auto origIt = _mapBaseRels.find( origGid );
    if ( origIt != _mapBaseRels.end( ))
    {
      auto destIt = origIt->second->find( destGid );
      if( destIt != origIt->second->end( ) )
      {
        const auto& relations = *destIt->second.aggregatedRelations;
        for( auto relationsIt : relations )
        {
          const auto& entityRelations = *relationsIt.second;
          for( auto relationIt : entityRelations )
          {
            relationIt.second.relationshipAggregatedProperties
              ->autoUpdateProperties( );
          }
        }
      }
    }
  }

  RelationshipProperties*
  RelationshipAggregatedOneToN::getRelationProperties( EntityGid entityOrig,
    EntityGid entityDest )
  {
    auto relationIt = _mapAggregatedRels.find( entityOrig );
    if( relationIt != _mapAggregatedRels.end( ))
    {
      auto relationMMIt =
        relationIt->second->find( entityDest );
      if( relationMMIt != relationIt->second->end( ))
      {
        return relationMMIt->second.relationshipAggregatedProperties.get( );
      }
    }
    return nullptr;
  }

  void Relationship::UpdateAggregatedRelations(
    RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
    RelationshipAggregatedOneToN& relAggregatedOneToNDest,
    const Entities& searchEntities )
  {
    relAggregatedOneToNOrig.clearRelations( );
    relAggregatedOneToNDest.clearRelations( );

    RelationshipOneToN& relOriginalOneToNOrig =
      *(relAggregatedOneToNOrig.baseRelationShip( ));
    RelationshipOneToOne& relHierarchyOneToOne =
      *(relAggregatedOneToNOrig.hierarchyRelationShip( ));

    for ( auto relationOrig : relOriginalOneToNOrig )
    {
      shift::Entity* entityOrig = searchEntities.at( relationOrig.first );
      for ( auto relationDest : relationOrig.second )
      {
        AggregateDependentRelations( relAggregatedOneToNOrig, relAggregatedOneToNDest,
          searchEntities, entityOrig, searchEntities.at( relationDest.first ),
          relationDest.second, relationDest.second, true, relHierarchyOneToOne );
      }
    }
  }

  void Relationship::ChangeEntityParent(
      RelationshipOneToN& relParentOf,
      RelationshipOneToOne& relChildOf,
      RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
      RelationshipAggregatedOneToN& relAggregatedOneToNDest,
      Entity* childEntity,
      Entity* newParentEntity,
      const Entities& searchEntities,
      Entities& rootEntities )
  {
    auto parentIt = relChildOf.find( childEntity->entityGid( ));
    if ( parentIt != relChildOf.end( ) && parentIt->second.entity > 0 )
    {
      auto parentEntity = searchEntities.at( parentIt->second.entity );
      shift::Relationship::relationBreak( relParentOf, relChildOf,
        parentEntity, childEntity );
      shift::Relationship::Establish( relParentOf, relChildOf,
        parentEntity, newParentEntity );
    }

    shift::Relationship::Establish( relParentOf, relChildOf,
      newParentEntity, childEntity );
    if (rootEntities.removeIfContains( childEntity ))
    {
      rootEntities.add( newParentEntity );
    }

    shift::Relationship::UpdateAggregatedRelations( relAggregatedOneToNOrig,
      relAggregatedOneToNDest, searchEntities );

  }

} // namespace shift
