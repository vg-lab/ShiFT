#include "Entities.h"


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
    assert( _map.find( entity->entityGid( )) == _map.end( ));
    _map[ entity->entityGid( ) ] = entity;
    _vector.push_back( entity );
    assert( _vector.size( ) == _map.size( ));
  }

}
