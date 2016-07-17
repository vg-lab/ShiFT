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
#include "ViewGraph.h"
#include "error.h"

namespace shift
{

  void ViewGraph::registerConnection( const ViewSPtr origin,
                                      const ViewSPtr dest )
  {
    SHIFT_CHECK_THROW(
      origin && dest,
      "shift::ViewGraph::registerConnection: origin or dest are nullptr" );
    _connections[ origin ].insert( dest );
  }

  const ViewGraph::TSetOfViews&
  ViewGraph::connections( const ViewSPtr origin ) const
  {
    if ( _connections.find( origin ) == _connections.end( ))
      SHIFT_THROW( "Could not find view" );

    return _connections.at( origin );
  }

  ViewGraph::TSetOfViews&
  ViewGraph::connections( const ViewSPtr origin )
  {
    if ( _connections.find( origin ) == _connections.end( ))
      SHIFT_THROW( "Could not find view" );

    return _connections.at( origin );
  }

}
