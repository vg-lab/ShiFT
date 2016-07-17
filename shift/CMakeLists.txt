source_group(\\ FILES CMakeLists.txt)

set( SHIFT_PUBLIC_HEADERS
  error.h
  Object.h
  Representation.h
  RepresentationCreator.h
  View.h
  ViewGraph.h
  )

set( SHIFT_HEADERS
  )

set( SHIFT_SOURCES
  Object.cpp
  ViewGraph.cpp
  )

set( SHIFT_LINK_LIBRARIES
  PUBLIC FiReS
  )

common_library( ShiFT )
