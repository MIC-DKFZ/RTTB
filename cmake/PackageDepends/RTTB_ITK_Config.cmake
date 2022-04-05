#-----------------------------------------------------------------------------
# Find ITK.
#-----------------------------------------------------------------------------

FIND_PACKAGE(ITK 5.2 REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${ITK_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES ${ITK_LIBRARIES})

LINK_DIRECTORIES(${ITK_LIBRARY_DIRS})

OPTION(RTTB_USE_SYSTEM_GDCM "Activate checker to choose a GDCM installation that should be linked with ITK. (This is needed e.g. when building against an ITK that is distributed with MITK. MITK uses its own GDCM and not the one distributed with ITK.)" OFF)     

IF (RTTB_USE_SYSTEM_GDCM)
  MESSAGE (STATUS "RTTB uses system GDCM instead of ITK distribution.")
  FIND_PACKAGE(GDCM PATHS ${ITK_GDCM_DIR} ${GDCM_DIR} REQUIRED)
  INCLUDE(${GDCM_USE_FILE})
  LIST(APPEND ALL_INCLUDE_DIRECTORIES ${GDCM_INCLUDE_DIRS})
  LIST(APPEND ALL_LIBRARIES ${GDCM_LIBRARIES})	    
ENDIF (RTTB_USE_SYSTEM_GDCM)

OPTION(RTTB_USE_SYSTEM_HDF5 "Activate checker to choose a HDF5 installation that should be linked with ITK. (This is needed e.g. when building against an ITK that is distributed with MITK. MITK uses its own HDF5 and not the one distributed with ITK.)" OFF)     

IF (RTTB_USE_SYSTEM_HDF5)
  MESSAGE (STATUS "RTTB uses system HDF5 instead of ITK distribution.")
  FIND_PACKAGE(HDF5 PATHS ${HDF5_DIR} REQUIRED)
  LIST(APPEND ALL_INCLUDE_DIRECTORIES ${HDF5_INCLUDE_DIRS})
  LIST(APPEND ALL_LIBRARIES ${HDF5_LIBRARIES})	    
ENDIF (RTTB_USE_SYSTEM_HDF5)


CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/ITKConfig.cmake.in ${RTTB_MODULES_CONF_DIR}/ITKConfig.cmake @ONLY)
