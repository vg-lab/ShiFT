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
#ifndef __SHIFT__RELATIONSHIP__
#define __SHIFT__RELATIONSHIP__

#include "Entity.h"
#include <shift/api.h>
#include <unordered_set>
#include <unordered_map>
#include <assert.h>

namespace shift
{

  class RelationshipProperties
    : public Properties
  {
  public:
    RelationshipProperties( void ) {}
    SHIFT_API virtual RelationshipProperties* create( void ) const
    { return nullptr; };
    virtual ~RelationshipProperties( void ) {}
  };


  class RelationshipOneToOne;
  class RelationshipOneToN;
  class RelationshipNToN;

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

    SHIFT_API Relationship( void );
    SHIFT_API virtual ~Relationship( void ) {}
    SHIFT_API TCardinality cardinality( void ) const;
    SHIFT_API virtual RelationshipOneToOne* asOneToOne( void );
    SHIFT_API virtual RelationshipOneToN* asOneToN( void );
    SHIFT_API virtual RelationshipNToN* asNToN( void );

    static void Establish( RelationshipOneToN& relOneToN,
                           RelationshipOneToOne& relOneToOne,
                           Entity* entityOrig, Entity* entityDest )
    {
      assert( entityOrig && entityDest );
      Establish( relOneToN, relOneToOne,
                 entityOrig->entityGid( ), entityDest->entityGid( ));
    }

    SHIFT_API
    static void Establish( RelationshipOneToN& relOneToN,
                           RelationshipOneToOne& relOneToOne,
                           Entity::EntityGid entityOrig,
                           Entity::EntityGid entityDest );

    static void Establish( RelationshipOneToN& relOneToNOrig,
                           RelationshipOneToN& relOneToNDest,
                           Entity* entityOrig, Entity* entityDest )
    {
      assert( entityOrig && entityDest );
      Establish( relOneToNOrig, relOneToNDest,
                 entityOrig->entityGid( ), entityDest->entityGid( ));
    }

    SHIFT_API
    static void Establish( RelationshipOneToN& relOneToNOrig,
                           RelationshipOneToN& relOneToNDest,
                           Entity::EntityGid entityOrig,
                           Entity::EntityGid entityDest );

  protected:
    TCardinality _cardinality;

  };

  typedef struct
  {
    Entity::EntityGid entity;
    RelationshipProperties* properties;
  } RelationshipOneToOneDest;

  class RelationshipOneToOne
    : public Relationship
    , public std::unordered_map< Entity::EntityGid,
                                 RelationshipOneToOneDest >
  {
  public:
    SHIFT_API RelationshipOneToOne( void );
    SHIFT_API virtual RelationshipOneToOne* asOneToOne( void );
  };

  // typedef struct
  // {
  //   std::unordered_set< Entity::EntityGid > entities;
  //   RelationshipProperties* properties;
  // } RelationshipOneToNDest;
  typedef std::pair< Entity::EntityGid, RelationshipProperties* >
  RelationshipOneToNDest;

  class RelationshipOneToN
    : public Relationship
    , public std::unordered_map< Entity::EntityGid,
                                 std::unordered_multimap< Entity::EntityGid,
                                                          RelationshipProperties* >>
  {
  public:
    SHIFT_API RelationshipOneToN( void );
    SHIFT_API virtual RelationshipOneToN* asOneToN( void );

  };

    class RelationshipNToN
    : public Relationship
    , public std::vector< std::tuple<
                            std::unordered_set< Entity::EntityGid >,
                            std::unordered_set< Entity::EntityGid >,
                            RelationshipProperties >>
    {
  public:
    SHIFT_API RelationshipNToN( void );
    SHIFT_API virtual RelationshipNToN* asNToN( void );
  };

} // namespace shift
#endif // __SHIFT__RELATIONSHIP__
