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
#ifndef __SHIFT__REPRESENTATION_CREATOR__
#define __SHIFT__REPRESENTATION_CREATOR__

#include <vector>
#include <unordered_map>
#include <set>
#include "Entities.h"
#include "Representation.h"

namespace shift
{
  typedef std::unordered_map< shift::Entity*,
                              std::set< shift::Representation* >>
    TEntitiesToReps;
  typedef std::unordered_map< shift::Representation*,
                              std::set< shift::Entity* >>
    TRepsToEntities;

  typedef std::tuple< shift::Representation*,
                      shift::Entity*,
                      shift::Entity*,
                      shift::Representation*,
                      shift::Representation*> TEntityRelationRep;

  struct pairhash {
  public:
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
      return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
  };
  typedef std::unordered_multimap< std::pair< unsigned int, unsigned int >,
                                   TEntityRelationRep,
                                   pairhash > TRelatedEntitiesReps;

  typedef std::unordered_map< unsigned int,
      std::pair< shift::Entity*, shift::Representation* >> TGidToEntitiesReps;


  class RepresentationCreator
  {


  public:
    virtual ~RepresentationCreator( void ) {};

    virtual void create(
      const shift::Entities& entities,
      shift::Representations& representations,
      shift::TEntitiesToReps& entitiesToReps,
      shift::TRepsToEntities& repsToEntities,
      shift::TGidToEntitiesReps& gidsToEntitiesReps,
      bool linkEntitiesToReps = false,
      bool linkRepsToObjs = false ) = 0;

    virtual void generateRelations(
      const shift::Entities& entities,
      const shift::TGidToEntitiesReps& gidsToEntitiesReps,
      shift::TRelatedEntitiesReps& relatedEntitiesReps,
      shift::Representations& relatedEntities,
      shift::RelationshipOneToN* relatedElements ) = 0;

    virtual void generateRelations(
      const shift::Entities& entities,
      const shift::TGidToEntitiesReps& gidsToEntitiesReps,
      shift::TRelatedEntitiesReps& relatedEntitiesReps,
      shift::Representations& relatedEntities,
      shift::RelationshipAggregatedOneToN* relatedElements )  = 0;

    virtual void clear( void ) { }

    // Should return true if any changes have occurred
    virtual bool entityUpdatedOrCreated( shift::Entity* /* entity */ )
    {
      return false;
    }

    // Should return true if any changes have occurred
    virtual bool relationshipUpdatedOrCreated(
      shift::RelationshipProperties* /* relProperties */ )
    {
      return false;
    }

  };

}

#endif // __SHIFT__REPRESENTATION_CREATOR__
