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

  class RepresentationCreator
  {


  public:
    virtual ~RepresentationCreator( void ) {};

    virtual void create(
      const shift::Entities& entities,
      shift::Representations& representations,
      shift::TEntitiesToReps& entitiesToReps,
      shift::TRepsToEntities& repsToEntities,
      bool linkEntitiesToReps = false,
      bool linkRepsToObjs = false ) = 0;

  };

}

#endif // __SHIFT__REPRESENTATION_CREATOR__
