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
#ifndef __SHIFT__PROPERTIES__
#define __SHIFT__PROPERTIES__

#include <shift/api.h>
#include <fires/fires.h>


namespace shift
{

  class Properties : public fires::Object
  {
  public:
    typedef enum { SUBPROPERTY } PropertyConstraintType;

    typedef enum { SUM, MEAN, MIN, MAX, MAXPLUS1, COUNT } TAutoUpdatePropertyOp;
    typedef enum { EDITABLE, UNIQUE, HIDE } TPropertyFlag;


    virtual bool evalConstraint(
      const PropertyConstraintType& /* constraintType */,
      const std::string& /* propertyName */ ) const
    {
      return true;
    }

  };

}
#endif // __SHIFT__PROPERTIES__
