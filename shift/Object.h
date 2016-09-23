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
#ifndef __SHIFT__OBJECT__
#define __SHIFT__OBJECT__

#include "definitions.h"
#include <shift/api.h>
#include <fires/fires.h>

namespace shift
{

  class Object
    : public fires::Object
  {
  public:
    SHIFT_API
    Object( void );

    SHIFT_API
    virtual ~Object( void );
  };

  typedef std::vector< shift::Object* > Objects;


}
#endif // __SHIFT__OBJECT__
