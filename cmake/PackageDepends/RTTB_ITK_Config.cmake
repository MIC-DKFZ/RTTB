#-----------------------------------------------------------------------------
# Find ITK.
#-----------------------------------------------------------------------------

FIND_PACKAGE(ITK 4.4 REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${ITK_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES ${ITK_LIBRARIES})

LINK_DIRECTORIES(${ITK_LIBRARY_DIRS})

CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/ITKConfig.cmake.in ${RTTB_MODULES_CONF_DIR}/ITKConfig.cmake @ONLY)
