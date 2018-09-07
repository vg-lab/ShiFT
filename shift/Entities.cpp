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

    SHIFT_CHECK_THROW( _map.find( entity->entityGid( )) == _map.end( ),
      "ERROR: element already in map" );
    _map.insert( std::make_pair( entity->entityGid( ),entity ));
    _vector.push_back( entity );
    SHIFT_CHECK_THROW( _vector.size( ) == _map.size( ),
      "ERROR: size incoherence between map and vector" );
  }

  void Entities::remove( Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ));
    SHIFT_CHECK_THROW( mapIt != _map.end( ),
      "ERROR: element not contained in map" );
    _map.erase( mapIt );
    auto vectorIt = std::find( _vector.begin( ),_vector.end( ), entity );
    SHIFT_CHECK_THROW( vectorIt != _vector.end( ),
      "ERROR: element not contained in map" );
    _vector.erase( vectorIt );
    SHIFT_CHECK_THROW(_vector.size( ) == _map.size( ),
      "ERROR: size incoherence between map and vector" );
  }

  bool  Entities::addIfNotContains( Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ));
    if ( mapIt == _map.end( ))
    {
      _map.insert( std::make_pair( entity->entityGid( ),entity ));
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

  bool  Entities::removeIfContains( Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ) );
    if ( mapIt != _map.end( ))
    {
      _map.erase( mapIt );
      auto vectorIt = std::find( _vector.begin( ),_vector.end( ), entity );
      SHIFT_CHECK_THROW( vectorIt != _vector.end( ),
        "ERROR: element not contained in map" );
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

  bool  Entities::contains( Entity* entity ) const
  {
    return _map.find( entity->entityGid( )) != _map.end( );
  }

  void Entities::addRelatedEntitiesOneToN( RelationshipOneToN& relation,
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

  void Entities::addRelatedEntitiesOneToOne( RelationshipOneToOne& relation,
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

  void Entities::removeRelatedEntitiesOneToN( RelationshipOneToN& relation,
    const Entity* entity, const Entities& searchEntities, int depthLevel)
  {
    const auto &relatives = relation[ entity->entityGid( ) ];
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

  void Entities::removeRelatedEntitiesOneToOne( RelationshipOneToOne& relation,
    const Entity* entity, const Entities& searchEntities, int depthLevel)
  {
    for ( auto parent = relation[ entity->entityGid( ) ].entity;
          parent != 0; --depthLevel )
    {
      Entity* relatedEntity = searchEntities.at( parent );
      if ( removeIfContains( relatedEntity ) && depthLevel != 1 )
      {
        parent = relation[ parent ].entity;
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
