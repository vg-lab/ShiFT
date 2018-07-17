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
      "ERROR: size incoherence between map and vector");
  }

  void Entities::remove( Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ));
    SHIFT_CHECK_THROW( mapIt != _map.end( ),
      "ERROR: element not contained in map");
    _map.erase( mapIt );
    auto vectorIt = std::find( _vector.begin( ),_vector.end( ), entity );
    SHIFT_CHECK_THROW( vectorIt != _vector.end( ),
      "ERROR: element not contained in map");
    _vector.erase( vectorIt );
    SHIFT_CHECK_THROW(_vector.size( ) == _map.size( ),
      "ERROR: size incoherence between map and vector");
  }

  bool  Entities::addIfNotContains( Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ));
    if ( mapIt == _map.end( ))
    {
      _map.insert( std::make_pair( entity->entityGid( ),entity ));
      _vector.push_back( entity );
      SHIFT_CHECK_THROW( _vector.size( ) == _map.size( ),
        "ERROR: size incoherence between map and vector");
      return true;
    }
    else
    {
      return false;
    }
  }

  bool  Entities::removeIfContains( Entity* entity )
  {
    auto mapIt = _map.find( entity->entityGid( ));
    if ( mapIt != _map.end( ))
    {
      _map.erase( mapIt );
      auto vectorIt = std::find( _vector.begin( ),_vector.end( ), entity );
      SHIFT_CHECK_THROW( vectorIt != _vector.end( ),
                         "ERROR: element not contained in map");
      _vector.erase( vectorIt );
      SHIFT_CHECK_THROW(_vector.size( ) == _map.size( ),
                        "ERROR: size incoherence between map and vector");
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
}
