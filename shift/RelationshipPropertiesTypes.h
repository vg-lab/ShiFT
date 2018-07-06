/*
 * Copyright (c) 2014-2016 GMRV/URJC/UPM.
 *
 * Authors: Juan Jose Garcia Cantero <juanjose.garcia@urjc.es>
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
#ifndef __SHIFT_RELATIONSHIP_PROPERTIES_TYPES__
#define __SHIFT_RELATIONSHIP_PROPERTIES_TYPES__

#include "Relationship.h"
#include <shift/api.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace shift
{
  class RelationshipPropertiesTypes
  {
  public:

    typedef std::multimap< std::string, std::string > TRelationshipConstraints;
    typedef std::pair< TRelationshipConstraints::const_iterator,
      TRelationshipConstraints::const_iterator > TRelConstraintsRange;
    typedef TRelConstraintsRange rel_constr_range;

    typedef std::map< std::string, TRelationshipConstraints* > TConstraintsContainer;
    typedef std::pair< TConstraintsContainer::const_iterator,
      TConstraintsContainer::const_iterator > TConstraintsRange;
    typedef TConstraintsRange constraints_range;

    typedef std::unordered_map< std::string, RelationshipProperties* >
      TRelationshipPropertiesTypes;

    virtual ~RelationshipPropertiesTypes( void ){ }

    SHIFT_API static constraints_range constraints( void );
    SHIFT_API static rel_constr_range constraints(
      const std::string& relationshipName );
    SHIFT_API static rel_constr_range constraints(
      const std::string& relationshipName, const std::string& entityName );

    SHIFT_API static bool isConstrained( const std::string& relationshipName,
      const std::string& srcEntity, const std::string& dstEntity );

    SHIFT_API const TRelationshipPropertiesTypes&
      relationshipPropertiesTypes( void );

    RelationshipProperties* getRelationshipProperties(
      const std::string& relationshipName_ );

  protected:
    TRelationshipPropertiesTypes _relationshipPropertiesTypes;
    static TConstraintsContainer* _constraints;
    static void addConstraint( const std::string& constraintTypeName,
      const std::string& srcEntityTypeName,
      const std::string& dstEntityTypeName );
  };

}

#endif
