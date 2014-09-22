#-----------------------------------------------------------------------------
# Find Litmus
#-----------------------------------------------------------------------------
IF(DEFINED Litmus_DIR)
  IF(NOT IS_ABSOLUTE ${Litmus_DIR})
    SET(Litmus_DIR "${RTToolbox_BINARY_DIR}/${Litmus_DIR}")
  ENDIF(NOT IS_ABSOLUTE ${Litmus_DIR})

  SET(Litmus_DIR ${Litmus_DIR} CACHE PATH "Litmus binary dir.")
ENDIF(DEFINED Litmus_DIR)




FIND_PACKAGE(Litmus)
IF(Litmus_FOUND)
  INCLUDE(${Litmus_USE_FILE})
ELSE(Litmus_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without Litmus.  Please set Litmus_DIR.")
ENDIF(Litmus_FOUND)

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${Litmus_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES LitmusCommon)

LINK_DIRECTORIES(${Litmus_LIBRARY_DIRS}) 
