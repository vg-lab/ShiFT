/*
 * Copyright (c) 2014-2016 GMRV/URJC/UPM.
 *
 * Authors: Pablo Toharia <pablo.toharia@upm.es>
 *          Iago Calvo Lista <i.calvol@alumnos.urjc.es>
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
#ifndef __SHIFT__RELATIONSHIP_PROPERTIES__
#define __SHIFT__RELATIONSHIP_PROPERTIES__

#include "definitions.h"
#include "Properties.h"
#include <shift/api.h>
#include <vector>

namespace shift
{
  class Entity;
  class Entities;

  class RelationshipProperties : public Properties
  {
    public:
    typedef enum { SUM, MEAN, MIN, MAX, COUNT } TAutoUpdatePropertyOp;
    typedef enum { EDITABLE } TPropertyFlag;
    typedef std::unordered_map< std::string,
      std::set< RelationshipProperties::TPropertyFlag >> TPropertiesFlagsMap;

    RelationshipProperties( void ) { }
    SHIFT_API virtual RelationshipProperties* create( void ) const
    { return nullptr; }
    SHIFT_API
    virtual bool hasPropertyFlag( const std::string& /* propertyLabel */,
      TPropertyFlag /* flag */ ) const;
    SHIFT_API virtual const std::string& typeName( void ) const;
    virtual ~RelationshipProperties( void ) { }
    SHIFT_API
    virtual void autoUpdateProperties( void ){ };
    virtual void autoUpdateProperty( fires::Object* /* obj */,
      const std::string& /* propertyLabel */ ) { };
    SHIFT_API void autoUpdatePropertyWithRelatedRelations(
      const std::vector< std::string >& relatedEntitiesNames_,
      RelationshipProperties::TAutoUpdatePropertyOp op_,
      const std::string& origPropertyLabel_,
      const std::string& destPropertyLabel_);
    SHIFT_API virtual void relatedRelations(
      std::vector< RelationshipProperties* >* relatedRelations_ );

    virtual void setRelatedDependencies(
      shift::RelationshipProperties* /* dependency */ ) {}
    virtual void removeRelatedDependencies(
      shift::RelationshipProperties* /* dependency */ ) {}
    private:
    std::vector< RelationshipProperties* >* _relatedRelations = nullptr;
  };

} // namespace shift
#endif // __SHIFT__RELATIONSHIP_PROPERTIES__