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
    assert( entityOrig && entityDest );

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
    assert( entityOrig && entityDest );
    auto entityOrigGid = entityOrig->entityGid( );
    auto entityDestGid = entityDest->entityGid( );

    relOneToNOrig[ entityOrigGid ].insert(
      RelationshipOneToNDest( entityDestGid, propertiesOrig ));
    relOneToNDest[ entityDestGid ].insert(
      RelationshipOneToNDest( entityOrigGid, propertiesDest ));

  }


} // namespace shift
