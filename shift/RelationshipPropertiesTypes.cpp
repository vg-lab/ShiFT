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

#include "RelationshipPropertiesTypes.h"

namespace shift
{

  RelationshipPropertiesTypes::TConstraintsContainer*
    RelationshipPropertiesTypes::_constraints = new TConstraintsContainer( );

  RelationshipPropertiesTypes::constraints_range
  RelationshipPropertiesTypes::constraints( void )
  {
    return std::make_pair( _constraints->begin( ), _constraints->end( ));
  }

  RelationshipPropertiesTypes::rel_constr_range
    RelationshipPropertiesTypes::constraints(
    const std::string& relationshipName )
  {
    auto relIt = _constraints->find( relationshipName );
    assert( relIt != _constraints->end( ) );
    return std::make_pair( relIt->second->begin( ),
      relIt->second->end( ));
  }

  RelationshipPropertiesTypes::rel_constr_range
    RelationshipPropertiesTypes::constraints(
    const std::string& relationshipName, const std::string& entityName )
  {
    auto relIt = _constraints->find( relationshipName );
    assert( relIt != _constraints->end( ));
    return relIt->second->equal_range( entityName );
  }

  bool RelationshipPropertiesTypes::isConstrained(
    const std::string& relationshipName,
    const std::string& srcEntity, const std::string& dstEntity )
  {
    auto relIt = _constraints->find( relationshipName );
    assert( relIt != _constraints->end( ) );
    auto results = relIt->second->equal_range( srcEntity );
    for( auto dstIt = results.first; dstIt != results.second; ++dstIt )
      if( dstIt->second == dstEntity )
        return true;
    return false;
  }

  const RelationshipPropertiesTypes::TRelationshipPropertiesTypes&
    RelationshipPropertiesTypes::relationshipPropertiesTypes( void )
  {
    return _relationshipPropertiesTypes;
  }

  RelationshipProperties*
    RelationshipPropertiesTypes::getRelationshipProperties(
    const std::string& relationshipName_ )
  {
    auto relPropIt = _relationshipPropertiesTypes.find( relationshipName_ );
    if( relPropIt != _relationshipPropertiesTypes.end( ))
      return relPropIt->second;
    else
      return nullptr;
  }

  void RelationshipPropertiesTypes::addConstraint(
    const std::string& constraintTypeName, const std::string& srcEntityTypeName,
    const std::string& dstEntityTypeName )
  {
    auto it = _constraints->find( constraintTypeName );
    RelationshipPropertiesTypes::TRelationshipConstraints* container;
    if( it == _constraints->end( ) )
    {
      container = new RelationshipPropertiesTypes::TRelationshipConstraints( );
      _constraints ->insert( std::pair< std::string,
        RelationshipPropertiesTypes::TRelationshipConstraints* > (
        constraintTypeName, container ));
    }else{
      container =  it->second;
    }
    container->insert(std::make_pair( srcEntityTypeName, dstEntityTypeName ));
  }
}