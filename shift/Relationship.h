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
#ifndef __SHIFT__RELATIONSHIP__
#define __SHIFT__RELATIONSHIP__

#include "Entities.h"
#include "Entity.h"
#include "RelationshipProperties.h"
#include "Properties.h"
#include <shift/api.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace shift
{
  class RelationshipOneToOne;
  class RelationshipOneToN;
  class RelationshipNToN;
  class RelationshipAggregatedOneToN;

  class Relationship
  {
  public:
    typedef enum
    {
      UNDEFINED,
      ONE_TO_ONE,
      ONE_TO_N,
      N_TO_N
    } TCardinality;

    SHIFT_API Relationship( const std::string& name = "" );
    SHIFT_API virtual ~Relationship( void ) {}
    SHIFT_API TCardinality cardinality( void ) const;
    SHIFT_API virtual RelationshipOneToOne* asOneToOne( void );
    SHIFT_API virtual RelationshipOneToN* asOneToN( void );
    SHIFT_API virtual RelationshipNToN* asNToN( void );
    SHIFT_API virtual RelationshipAggregatedOneToN* asAggregatedOneToN( void );
    const std::string& name( void ) const { return _name; }
    SHIFT_API virtual void clearRelations( );

    SHIFT_API
    static void relationBreak( RelationshipOneToN& relOneToNOrig,
      RelationshipOneToN& relOneToNDest,
      Entity* entityOrig,Entity* entityDest );

    SHIFT_API
    static void relationBreak( RelationshipOneToN& relOneToNOrig,
      RelationshipOneToOne& relOneToOneDest,
      Entity* entityOrig,Entity* entityDest );

    SHIFT_API
    static void Establish( RelationshipOneToN& relOneToN,
      RelationshipOneToOne& relOneToOne,
      Entity* entityOrig, Entity* entityDest );

    SHIFT_API
    static void Establish( RelationshipOneToN& relOneToNOrig,
      RelationshipOneToN& relOneToNDest, Entity* entityOrig,
      Entity* entityDest, RelationshipProperties* propertiesOrig = nullptr,
      RelationshipProperties* propertiesDest = nullptr );

    SHIFT_API
    static void BreakAnAggregatedRelation(
      RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
      RelationshipAggregatedOneToN& relAggregatedOneToNDest,
      Entity* entityOrig, Entity* entityDest );

    SHIFT_API
    static void UpdateAggregatedRelations(
      RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
      RelationshipAggregatedOneToN& relAggregatedOneToNDest,
      const Entities& searchEntities );

    SHIFT_API
    static void ChangeEntityParent(
      RelationshipOneToN& relParentOf,
      RelationshipOneToOne& relChildOf,
      RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
      RelationshipAggregatedOneToN& relAggregatedOneToNDest,
      Entity* childEntity,
      Entity* newParentEntity,
      const Entities& searchEntities,
      Entities& rootEntities );

    SHIFT_API
    static void EstablishAndAggregate(
      RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
      RelationshipAggregatedOneToN& relAggregatedOneToNDest,
      const Entities& searchEntities, Entity* entityOrig,
      Entity* entityDest, RelationshipProperties* propertiesOrig = nullptr,
      RelationshipProperties* propertiesDest = nullptr,
      bool recalcProperties = true );

  protected:
    TCardinality _cardinality;
    std::string _name;

    SHIFT_API
    static void Establish( RelationshipAggregatedOneToN &relOneToNOrig,
      RelationshipAggregatedOneToN &relOneToNDest, Entity* entityOrig,
      Entity* entityDest, Entity* entityBaseOrig,  Entity* entityBaseDest,
      RelationshipProperties* propertiesOrig = nullptr,
      RelationshipProperties* propertiesDest  = nullptr,
      const bool recalcProperties = true );


    SHIFT_API
    static void AggregateDependentRelations(
      RelationshipAggregatedOneToN& relAggregatedOneToNOrig,
      RelationshipAggregatedOneToN& relAggregatedOneToNDest,
      const Entities& searchEntities, Entity* entityOrig,
      Entity* entityDest, RelationshipProperties* propertiesOrig,
      RelationshipProperties* propertiesDest, bool recalcProperties,
      const RelationshipOneToOne& relHierarchyOneToOne );
  };

  typedef struct
  {
    EntityGid entity;
    RelationshipProperties* properties;
  } RelationshipOneToOneDest;

  class RelationshipOneToOne
    : public Relationship
    , public std::unordered_map< EntityGid, RelationshipOneToOneDest >
  {
  public:
    SHIFT_API RelationshipOneToOne( const std::string& name );
    SHIFT_API virtual RelationshipOneToOne* asOneToOne( void ) override;
    SHIFT_API void clearRelations( void ) override;
  };

  // typedef struct
  // {
  //   std::unordered_set< EntityGid > entities;
  //   RelationshipProperties* properties;
  // } RelationshipOneToNDest;
  typedef std::pair< EntityGid, RelationshipProperties* >
  RelationshipOneToNDest;
  typedef std::unordered_multimap< EntityGid, RelationshipProperties* >
      RelationshipOneToNMapDest;

  class RelationshipOneToN
    : public Relationship
    , public std::unordered_map< EntityGid, RelationshipOneToNMapDest>
  {
  public:
    SHIFT_API RelationshipOneToN( const std::string& name );
    SHIFT_API virtual RelationshipOneToN* asOneToN( void ) override;
    SHIFT_API RelationshipProperties* getRelationProperties(
      EntityGid entityOrig, EntityGid entityDest );
    SHIFT_API void clearRelations( void ) override;

  };

  struct RelationshipAggregatedOneToNProperties
  {
    std::shared_ptr< std::vector< RelationshipProperties* >> basedProperties;
    std::shared_ptr< RelationshipProperties > relationshipAggregatedProperties;
    RelationshipAggregatedOneToNProperties( )
    {
    }
    RelationshipAggregatedOneToNProperties(
      std::vector< RelationshipProperties* >* basedProperties_,
      RelationshipProperties* relationshipAggregatedProperties_)
     : basedProperties( basedProperties_)
     , relationshipAggregatedProperties( relationshipAggregatedProperties_ )
    {
    }

    RelationshipAggregatedOneToNProperties(
        std::shared_ptr< std::vector< RelationshipProperties* >> basedProperties_,
        std::shared_ptr< RelationshipProperties > relationshipAggregatedProperties_)
        : basedProperties( basedProperties_)
        , relationshipAggregatedProperties( relationshipAggregatedProperties_ )
    {
    }

    ~RelationshipAggregatedOneToNProperties( )
    {
    }

  };


  typedef std::pair< EntityGid,
    RelationshipAggregatedOneToNProperties > AggregatedOneToNDependentProperty;
  typedef std::vector< AggregatedOneToNDependentProperty >
    AggregatedOneToNDependentProperties;
  typedef std::unordered_map< EntityGid,
    std::shared_ptr< AggregatedOneToNDependentProperties >>
    AggregatedOneToNDependentRelations;

  struct RelationshipBasesAggregatedOneToN
  {
    std::shared_ptr< AggregatedOneToNDependentRelations > aggregatedRelations;
    RelationshipProperties* relationProperties;

    RelationshipBasesAggregatedOneToN(
      std::shared_ptr< AggregatedOneToNDependentRelations > aggregatedRelations_,
      RelationshipProperties* relationProperties_ )
    : aggregatedRelations( aggregatedRelations_ ),
      relationProperties ( relationProperties_ )
    {
    }

    ~RelationshipBasesAggregatedOneToN( )
    {
    }
  };
  typedef std::unordered_map< EntityGid,
    RelationshipAggregatedOneToNProperties > AggregatedOneToNAggregatedDests;
  typedef std::unordered_map< EntityGid,
    std::shared_ptr<  AggregatedOneToNAggregatedDests >>
    AggregatedOneToNAggregatedRels;
  typedef std::unordered_map< EntityGid, RelationshipBasesAggregatedOneToN >
    AggregatedOneToNBaseDests;
  typedef std::unordered_map< EntityGid,
    std::shared_ptr< AggregatedOneToNBaseDests > > AggregatedOneToNBaseRels;


  class RelationshipAggregatedOneToN
    : public Relationship
  {
  public:
    SHIFT_API RelationshipAggregatedOneToN( const std::string& name,
     RelationshipProperties* baseAggregatedProperties_,
     RelationshipOneToOne* hierarchyRelationShip_,
     RelationshipOneToN* baseRelationShip );

    SHIFT_API virtual void addBaseRelation( EntityGid entityOrig,
      EntityGid entityDest, EntityGid entityBaseOrig, EntityGid entityBaseDest,
      RelationshipProperties* relationshipOrigProperties,
      const bool recalcProperties = true,
      const std::string& entityOrigName = "",
      const std::string& entityDestName="" );
    SHIFT_API virtual void removeDependentRelation(
      EntityGid entityOrigRemove, EntityGid entityDestRemove );

    SHIFT_API
    virtual RelationshipAggregatedOneToN* asAggregatedOneToN( void ) override;
    SHIFT_API RelationshipOneToOne* hierarchyRelationShip( ) const;
    SHIFT_API RelationshipOneToN* baseRelationShip( ) const;
    SHIFT_API AggregatedOneToNAggregatedRels& mapAggregatedRels( );
    SHIFT_API void updateDependentRelations(EntityGid origGid,
      EntityGid destGid );
    SHIFT_API RelationshipProperties* getRelationProperties(
      EntityGid entityOrig, EntityGid entityDest );
    SHIFT_API void clearRelations( void ) override;

  private:

    //This map it's use in representation to search the aggregated
    //relations between two entities
    AggregatedOneToNAggregatedRels _mapAggregatedRels;
    //This map allows a search of the dependent aggregated relations of
    //relation allowing their quicker erase and modification
    AggregatedOneToNBaseRels _mapBaseRels;
    RelationshipProperties* _baseAggregatedProperties;
    RelationshipOneToOne* _hierarchyRelationShip;
    RelationshipOneToN* _baseRelationShip;

  };

  class RelationshipNToN
  : public Relationship
  , public std::vector< std::tuple< std::unordered_set< EntityGid >,
    std::unordered_set< EntityGid >, RelationshipProperties >>
  {
  public:
    SHIFT_API RelationshipNToN( const std::string& name );
    SHIFT_API virtual RelationshipNToN* asNToN( void ) override;
    SHIFT_API void clearRelations( void ) override;
  };

} // namespace shift
#endif // __SHIFT__RELATIONSHIP__
