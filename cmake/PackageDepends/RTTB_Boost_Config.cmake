IF(NOT Boost_FOUND)

  IF(DEFINED Boost_INCLUDE_DIR)
    IF(NOT IS_ABSOLUTE ${Boost_INCLUDE_DIR})
      SET(Boost_INCLUDE_DIR "${RTToolbox_BINARY_DIR}/${Boost_INCLUDE_DIR}")
    ENDIF(NOT IS_ABSOLUTE ${Boost_INCLUDE_DIR})
  ENDIF(DEFINED Boost_INCLUDE_DIR)

  FIND_PACKAGE(Boost REQUIRED)

  LIST(APPEND ALL_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
  # this is only necessary if we use more than just the headers
  LIST(APPEND ALL_LIBRARIES ${Boost_LIBRARIES})
  link_directories(${Boost_LIBRARY_DIRS})
	
  MARK_AS_ADVANCED(CLEAR Boost_INCLUDE_DIR)
	
ENDIF(NOT Boost_FOUND)

