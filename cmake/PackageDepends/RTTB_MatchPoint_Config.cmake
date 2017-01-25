#-----------------------------------------------------------------------------
# Find MatchPoint
#-----------------------------------------------------------------------------
FIND_PACKAGE(MatchPoint)
IF(MatchPoint_FOUND)
  INCLUDE(${MatchPoint_USE_FILE})
ELSE(MatchPoint_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without MATCHPOINT.  Please set MATCHPOINT_DIR.")
ENDIF(MatchPoint_FOUND)

IF(${MatchPoint_VERSION_MAJOR} EQUAL 0 AND ${MatchPoint_VERSION_MINOR} LESS 12)
  MESSAGE(FATAL_ERROR
          "Outdated MatchPoint version. Cannot build RTToolbox without sufficient MatchPoint version. MatchPoint 0.12 or above is needed.")
ENDIF()

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${MatchPoint_INCLUDE_DIRS} ${MatchPoint_ITK_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES MAPCore MAPAlgorithms MAPAlgorithmsITK MAPDeployment MAPIO ${MatchPoint_ITK_LIBRARIES})

LINK_DIRECTORIES(${MatchPoint_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS}) 