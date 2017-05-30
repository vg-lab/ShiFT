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

  Relationship::Relationship( void )
    : _cardinality( UNDEFINED )
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

  RelationshipOneToOne::RelationshipOneToOne( void )
  {
    _cardinality = ONE_TO_ONE;
  }

  RelationshipOneToOne* RelationshipOneToOne::asOneToOne( void )
  {
    return this;
  }

  RelationshipOneToN::RelationshipOneToN( void )
  {
    _cardinality = ONE_TO_N;
  }

  RelationshipOneToN* RelationshipOneToN::asOneToN( void )
  {
    return this;
  }

  RelationshipNToN::RelationshipNToN( void )
  {
    _cardinality = N_TO_N;
  }

  RelationshipNToN* RelationshipNToN::asNToN( void )
  {
    return this;
  }

  void Relationship::Establish( RelationshipOneToN& relOneToN,
                                RelationshipOneToOne& relOneToOne,
                                Entity::EntityGid entityOrig,
                                Entity::EntityGid entityDest,
                                RelationshipProperties* properties )
  {
    relOneToN[ entityOrig ].entities.insert( entityDest );
    relOneToOne[ entityDest ].entity = entityOrig;
    relOneToOne[ entityDest ].properties = properties;
  }

  void Relationship::Establish( RelationshipOneToN& relOneToNOrig,
                                RelationshipOneToN& relOneToNDest,
                                Entity::EntityGid entityOrig,
                                Entity::EntityGid entityDest,
                                RelationshipProperties* properties )
  {
    relOneToNOrig[ entityOrig ].entities.insert( entityDest );
    relOneToNDest[ entityDest ].entities.insert( entityOrig );
    relOneToNDest[ entityDest ].properties = properties;
  }


} // namespace shift
