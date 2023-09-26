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
#ifndef __SHIFT__VIEW_GRAPH__
#define __SHIFT__VIEW_GRAPH__

#include <unordered_map>
#include <unordered_set>
#include "View.h"


namespace shift
{
  class ViewGraph
  {

  public:

    typedef std::unordered_set< ViewSPtr > TSetOfViews;

    virtual ~ViewGraph( void ) {}
    void registerConnection( const ViewSPtr origin,
                             const ViewSPtr dest );
    const TSetOfViews& connections( const ViewSPtr origin ) const;
    TSetOfViews& connections( const ViewSPtr origin );

  protected:

    //! Container of which view can connect with
    std::unordered_map< ViewSPtr, TSetOfViews > _connections;

  };

}

#endif // __SHIFT__GRAPH__
