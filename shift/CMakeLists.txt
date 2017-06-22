source_group(\\ FILES CMakeLists.txt)

set( SHIFT_PUBLIC_HEADERS
  definitions.h
  error.h
  Entity.h
  Entities.h
  EntitiesTypes.h
  Relationship.h
  Representation.h
  RepresentationCreator.h
  View.h
  ViewGraph.h
  )

set( SHIFT_HEADERS
  )

set( SHIFT_SOURCES
  Entity.cpp
  Relationship.cpp
  Representation.cpp
  ViewGraph.cpp
  )

set( SHIFT_LINK_LIBRARIES
  PUBLIC FiReS
  )

common_library( ShiFT )
