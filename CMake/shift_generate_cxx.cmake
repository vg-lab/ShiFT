if( NOT PYTHON_EXECUTABLE )
  find_package( PythonInterp REQUIRED )
  message( STATUS  "Using python: ${PYTHON_EXECUTABLE}")
endif( )

find_file( SHIFT_CXX shiftCXX.py HINTS
  ${CMAKE_CURRENT_LIST_DIR}/../../../bin # from share/shift/CMake (!Windows)
  ${CMAKE_CURRENT_LIST_DIR}/../../bin # from shift/CMake (Windows)
  ${CMAKE_CURRENT_LIST_DIR}/../bin # from build dir
  )

if( NOT SHIFT_CXX )
  message( FATAL_ERROR "shiftCXX.py not found" )
endif( )

function( shift_generate_cxx
    __OBJ_TYPE__
    __JSON__
    __OUT_DIR__
    __NAME__
    __OBJS__ )

  set(__OUTPUT_FILES__ "")
  foreach( __OBJ__ ${__OBJS__} )
    set( __OUTPUT_FILES__ ${__OUTPUT_FILES__}
      ${__OUT_DIR__}/shift_${__OBJ__}.cpp
      ${__OUT_DIR__}/shift_${__OBJ__}.h )
  endforeach( )

  add_custom_command(
    COMMAND ${PYTHON_EXECUTABLE} ${SHIFT_CXX}
    ARGS
    ${__OBJ_TYPE__}
    ${__JSON__}
    ${__OUT_DIR__}
    ${__NAME__}
    ${__OBJS__}
    DEPENDS
    ${SHIFT_CXX}
    ${__JSON__}
    OUTPUT
    ${__OUTPUT_FILES__}
    )

endfunction( )
