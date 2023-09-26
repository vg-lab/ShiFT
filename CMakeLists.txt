# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   ShiFT
#   2016 (c) VG-Lab / URJC / UPM
#   vg-lab@vg-lab.es
#   www.vg-lab.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required( VERSION 3.1 FATAL_ERROR )
project( ShiFT VERSION 0.2.0 )
set( ShiFT_VERSION_ABI 8 )

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
  message( FATAL "no in source building allowed." )
endif()

list( APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common )

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake)
  message(FATAL_ERROR "CMake/common missing, run: git submodule update --init")
endif()

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_definitions( -DDEBUG )
endif( )

# description
set( SHIFT_DESCRIPTION
     "ShiFT - ScHematic vIsualization Framework and Tools" )
set( SHIFT_MAINTAINER "Pablo Toharia <pablo.toharia@upm.es>")
set( SHIFT_LICENSE LGPL)
set( COMMON_PROJECT_DOMAIN vg-lab.es )

include( Common )

common_find_package( Boost REQUIRED COMPONENTS unit_test_framework )
common_find_package( FiReS SYSTEM REQUIRED )
common_find_package_post( )

list( APPEND SHIFT_DEPENDENT_LIBRARIES FiReS )

add_subdirectory( bin )
add_subdirectory( shift )

configure_file(
  CMake/shift_generate_cxx.cmake
  ${PROJECT_BINARY_DIR}/${CMAKE_MODULE_INSTALL_PATH}/shift_generate_cxx.cmake
  COPYONLY
  )
install( FILES CMake/shift_generate_cxx.cmake
  DESTINATION ${CMAKE_MODULE_INSTALL_PATH} COMPONENT dev )


include( CPackConfig )
include( DoxygenRule )
