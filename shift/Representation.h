/*
 * Copyright (c) 2014-2016 VG-Lab/URJC/UPM.
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
#ifndef __SHIFT__REPRESENTATION__
#define __SHIFT__REPRESENTATION__

#include <vector>
#include <fires/fires.h>
#include <shift/api.h>

namespace shift
{

  class OpConfig
  {
  public:
    virtual ~OpConfig( void ) {}
  };

  class Representation
    : public fires::Object
  {
  public:
    virtual ~Representation( void ) {}

    SHIFT_API virtual void preRender( OpConfig* config = nullptr );
  };


  typedef std::vector< shift::Representation* > Representations;


}

#endif // __SHIFT__REPRESENTATION__
