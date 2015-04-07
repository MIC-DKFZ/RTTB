#-----------------------------------------------------------------------------
# Find ITK.
#-----------------------------------------------------------------------------



FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

IF(${ITK_VERSION_MAJOR} LESS 4 OR (${ITK_VERSION_MAJOR} EQUAL 4 AND ${ITK_VERSION_MINOR} LESS 4))
  MESSAGE(FATAL_ERROR
          "Outdated ITK version. Cannot build RTToolbox without sufficient ITK version. ITK 4.4 or above is needed.")
ENDIF(${ITK_VERSION_MAJOR} LESS 4 OR (${ITK_VERSION_MAJOR} EQUAL 4 AND ${ITK_VERSION_MINOR} LESS 4))

#LIST(APPEND ALL_INCLUDE_DIRECTORIES ${ITK_INCLUDE_DIRS})
#LIST(APPEND ALL_LIBRARIES ${ITK_LIBRARIES})

#LINK_DIRECTORIES(${ITK_LIBRARY_DIRS})    

CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/CMake/ITKConfig.cmake.in ${RTTB_MODULES_CONF_DIR}/ITKConfig.cmake @ONLY)
