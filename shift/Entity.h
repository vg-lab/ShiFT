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
#include "Properties.h"
#include "Entities.h"
#include <shift/api.h>
#include <fires/fires.h>

#include <unordered_map>

#include <typeinfo>

namespace shift
{

  class Entity
    : public Properties
  {
  public:

    typedef std::unordered_map< std::string,
      std::set< shift::Properties::TPropertyFlag >> TPropertiesFlagsMap;

    SHIFT_API
    Entity( void );

    SHIFT_API
    virtual ~Entity( void );

    SHIFT_API
    virtual void autoUpdateProperties( void ){ };

    SHIFT_API
    EntityGid entityGid( void ) const;

    SHIFT_API
    void entityGid( EntityGid entityGid_ );

    SHIFT_API
    static EntityGid shiftEntityGid( void );

    SHIFT_API
    static void shiftEntityGid( EntityGid shiftEntityGid_,
      const bool compare = false );

    virtual const std::string& typeName( void ) const = 0;

    SHIFT_API
    virtual Entity* create( void ) const = 0;

    virtual void createSubEntities(
      std::vector< shift::Entity* >& /* subentities */ ) const
    {}

    SHIFT_API
    virtual bool isSubEntity( void ) { return false; }

    SHIFT_API
    virtual bool isNotHierarchy( void ) { return false; }

    SHIFT_API
    inline virtual bool isSameEntityType( const shift::Entity* entity ) const
    {
      return typeid( *this ) == typeid( *entity );
    }

    SHIFT_API
    virtual bool hasPropertyFlag( const std::string& /* propertyLabel */,
      TPropertyFlag /* flag */ ) const
    {
      return false;
    }

    virtual void autoUpdateProperty( fires::Object* /* obj */,
                                     const std::string& /* propertyLabel */ ) {};

    SHIFT_API void autoUpdatePropertyWithRelatedEntities(
      const std::string& relName,
      const std::vector< std::string >& relatedEntitiesNames,
      TAutoUpdatePropertyOp op_,
      const std::string& origPropertyLabel,
      const std::string& destPropertyLabel );

    virtual void setRelatedDependencies( const std::string& /* relName */,
      shift::Entity* /* dependency */ ) { }
    virtual void removeRelatedDependencies( const std::string& /* relName */,
      shift::Entity* /* dependency */ ) { }

  protected:
    EntityGid _entityGid;

    static EntityGid _shiftEntityGid;

    void static autoCalcProperty(
      const TAutoUpdatePropertyOp& op_,
      const std::string& origPropertyLabel_,
      const std::string& destPropertyLabel_,
      fires::Objects& objs_,
      fires::Object& object_ );
  };


}
#endif // __SHIFT__ENTITY__
