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

#include <typeinfo>

namespace shift
{

  class Properties : public fires::Object
  {
  public:
    typedef enum
    {
      SUBPROPERTY
    } PropertyConstraintType;

    virtual bool evalConstraint(
      const PropertyConstraintType& /* constraintType */,
      const std::string& /* propertyName */ ) const
    {
      return true;
    }

  };

  class Entity
    : public Properties
  {
  public:
    typedef unsigned int EntityGid;

    SHIFT_API
    Entity( void );

    SHIFT_API
    virtual ~Entity( void );

    SHIFT_API
    EntityGid entityGid( void ) const;

    SHIFT_API
    virtual Entity* create( void ) const = 0;

    virtual void createSubEntities(
      std::vector< shift::Entity* >& /* subentities */ ) const
    {}

    SHIFT_API
    virtual bool isSubEntity( void ) { return false; }

    SHIFT_API
    inline virtual bool isSameEntityType( const shift::Entity* entity ) const
    {
      return typeid( *this ) == typeid( *entity );
    }

    typedef enum { EDITABLE, UNIQUE } TPropertyFlag;
    typedef std::unordered_map< std::string,
                                std::set< shift::Entity::TPropertyFlag >>
    TPropertiesFlagsMap;

    SHIFT_API
    virtual bool hasPropertyFlag( const std::string& /* propertyLabel */,
                                  TPropertyFlag /* flag */ ) const
    {
      return false;
    }

  protected:
    EntityGid _entityGid;
  };


}
#endif // __SHIFT__ENTITY__
