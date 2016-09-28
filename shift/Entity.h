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
#ifndef __SHIFT__ENTITY__
#define __SHIFT__ENTITY__

#include "definitions.h"
#include <shift/api.h>
#include <fires/fires.h>

#include <unordered_map>

namespace shift
{

  class Entity
    : public fires::Object
  {
  public:
    SHIFT_API
    Entity( void );

    SHIFT_API
    virtual ~Entity( void );

    SHIFT_API
    unsigned int entityGid( void ) const;

  protected:
    unsigned int _entityGid;
  };


}
#endif // __SHIFT__ENTITY__
