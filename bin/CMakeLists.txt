install( FILES shiftCXX.py DESTINATION bin COMPONENT bin )
configure_file(
  shiftCXX.py
  ${CMAKE_CURRENT_BINARY_DIR}
  COPYONLY
  )
