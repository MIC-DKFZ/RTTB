#-----------------------------------------------------------------------------
# Find DCMTK
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# This part is based on the findDCMTK of CMake 2.8.8
# it was patched because:
# 1. the find_library statement had an incomplete set
#    of search paths. If building DCMTK with Visual
#    Studio the libs are to be found in /lib/release
#    or /lib/debug
# 2. the module DCMRT was not included (but needed by RTToolbox)
#
IF(DEFINED DCMTK_DIR)
  IF(NOT IS_ABSOLUTE ${DCMTK_DIR})
    SET(DCMTK_DIR "${RTToolbox_BINARY_DIR}/${DCMTK_DIR}")
  ENDIF(NOT IS_ABSOLUTE ${DCMTK_DIR})
ENDIF(DEFINED DCMTK_DIR)

if(NOT DCMTK_FOUND AND NOT DCMTK_DIR)
	IF (NOT WIN32)
		set(DCMTK_DIR "/usr/include/dcmtk/")
	ENDIF(NOT WIN32)
endif()

#ensure that we always have the variable as cache, independent from
#setting it via gui or command line
set(DCMTK_DIR ${DCMTK_DIR} CACHE PATH "Root of DCMTK tree.")

IF(DEFINED DCMTK_SOURCE_DIR)
  IF(NOT IS_ABSOLUTE ${DCMTK_SOURCE_DIR})
    SET(DCMTK_SOURCE_DIR "${RTToolbox_BINARY_DIR}/${DCMTK_SOURCE_DIR}")
  ENDIF(NOT IS_ABSOLUTE ${DCMTK_SOURCE_DIR})
ENDIF(DEFINED DCMTK_SOURCE_DIR)

if(NOT DCMTK_FOUND AND NOT DCMTK_SOURCE_DIR)
  set(DCMTK_SOURCE_DIR ${DCMTK_DIR})
endif()

#ensure that we always have the variable as cache, independent from
#setting it via gui or command line
set(DCMTK_SOURCE_DIR ${DCMTK_SOURCE_DIR} CACHE PATH "Root of DCMTK tree.")

IF(NOT DEFINED RTTB_USE_MITK_DCMTK)
  OPTION(RTTB_USE_MITK_DCMTK "RTTB should use a DCMTK which is available in the MITK superbuild external projects structure." OFF)
  MARK_AS_ADVANCED(RTTB_USE_MITK_DCMTK)
ENDIF(NOT DEFINED RTTB_USE_MITK_DCMTK)

OPTION(RTTB_USE_ML_DCMTK "RTTB should use a DCMTK which is available in the MeVisLab package structure." OFF)
MARK_AS_ADVANCED(RTTB_USE_ML_DCMTK)


if(NOT DCMTK_FOUND)
  set(DCMTK_FOUND True)
  
  set(DCMTK_DEBUG_LIB_SEARCH_PATH 
    ${DCMTK_DIR}/${lib}/libsrc/Debug
    ${DCMTK_DIR}/${lib}/Debug
    ${DCMTK_DIR}/lib/Debug
    ${DCMTK_DIR}/Debug
    ${DCMTK_DIR}	
	)
  
  set(DCMTK_LIB_SEARCH_PATH 
    ${DCMTK_DIR}/${lib}/libsrc
    ${DCMTK_DIR}/${lib}/libsrc/Release
    ${DCMTK_DIR}/${lib}/Release
    ${DCMTK_DIR}/lib/Release
    ${DCMTK_DIR}/lib
    ${DCMTK_DIR}
	)

  set(DCMTK_LIB_SEARCH_NAMES
    dcmdata
    dcmimage
    dcmimgle
    dcmjpeg
    dcmnet
    dcmpstat
    dcmqrdb
    dcmdsig
    dcmsr
    dcmtls
    dcmrt
    ijg12
    ijg16
    ijg8
    ofstd
    oflog
	)

  if(${RTTB_USE_ML_DCMTK})
    set(DCMTK_DEBUG_LIB_SEARCH_PATH ${DCMTK_DIR}/lib )  
    set(DCMTK_LIB_SEARCH_PATH ${DCMTK_DIR}/lib )  
    set(DCMTK_LIB_SEARCH_NAMES ${DCMTK_LIB_SEARCH_NAMES} zlib)
  endif(${RTTB_USE_ML_DCMTK})



foreach(lib ${DCMTK_LIB_SEARCH_NAMES})
	
  set(debuglib ${lib})
  
  if(${RTTB_USE_MITK_DCMTK})
    set(debuglib ${lib}d)
  elseif(${RTTB_USE_ML_DCMTK})
    set(debuglib ${lib}_d)
  endif(${RTTB_USE_MITK_DCMTK})

  find_library(DCMTK_${lib}_DEBUG_LIBRARY
    ${debuglib}
    PATHS ${DCMTK_DEBUG_LIB_SEARCH_PATH})
  
  find_library(DCMTK_${lib}_LIBRARY
    ${lib}
    PATHS  ${DCMTK_LIB_SEARCH_PATH})	
	
  if((${UNIX}) AND (NOT DCMTK_${lib}_DEBUG_LIBRARY))
    set(DCMTK_${lib}_DEBUG_LIBRARY ${DCMTK_${lib}_LIBRARY} CACHE PATH "Path to a library" FORCE)
  endif((${UNIX}) AND (NOT DCMTK_${lib}_DEBUG_LIBRARY))

  mark_as_advanced(DCMTK_${lib}_LIBRARY)
  mark_as_advanced(DCMTK_${lib}_DEBUG_LIBRARY)
	
#  add_library(${lib} STATIC IMPORTED)
#  set_target_properties(${lib} PROPERTIES IMPORTED_LOCATION ${DCMTK_${lib}_LIBRARY} IMPORTED_LOCATION_DEBUG ${DCMTK_${lib}_DEBUG_LIBRARY})
  
  if(DCMTK_${lib}_LIBRARY )
    list(APPEND DCMTK_LIBRARIES optimized ${DCMTK_${lib}_LIBRARY})
  endif() 

  if(DCMTK_${lib}_DEBUG_LIBRARY)
    list(APPEND DCMTK_LIBRARIES debug ${DCMTK_${lib}_DEBUG_LIBRARY})
  endif() 
   
  if(NOT DCMTK_${lib}_LIBRARY)
	message(STATUS "Cannot find library DCMTK_${lib}_LIBRARY")
  endif()

  if(NOT DCMTK_${lib}_DEBUG_LIBRARY)
	message(STATUS "Cannot find library DCMTK_${lib}_DEBUG_LIBRARY")
  endif()

 endforeach()


set(DCMTK_config_TEST_HEADER osconfig.h)
set(DCMTK_dcmdata_TEST_HEADER dctypes.h)
set(DCMTK_dcmimage_TEST_HEADER dicoimg.h)
set(DCMTK_dcmimgle_TEST_HEADER dcmimage.h)
set(DCMTK_dcmjpeg_TEST_HEADER djdecode.h)
set(DCMTK_dcmnet_TEST_HEADER assoc.h)
set(DCMTK_dcmpstat_TEST_HEADER dcmpstat.h)
set(DCMTK_dcmqrdb_TEST_HEADER dcmqrdba.h)
set(DCMTK_dcmrt_TEST_HEADER drmdose.h)
set(DCMTK_dcmsign_TEST_HEADER sicert.h)
set(DCMTK_dcmsr_TEST_HEADER dsrtree.h)
set(DCMTK_dcmtls_TEST_HEADER tlslayer.h)
set(DCMTK_ofstd_TEST_HEADER ofstdinc.h)
set(DCMTK_oflog_TEST_HEADER oflog.h)

foreach(dir
    config
    dcmdata
    dcmimage
    dcmimgle
    dcmjpeg
    dcmnet
    dcmpstat
    dcmqrdb
	dcmrt
    dcmsign
    dcmsr
    dcmtls
    ofstd
	oflog)
  find_path(DCMTK_${dir}_INCLUDE_DIR
    ${DCMTK_${dir}_TEST_HEADER}
    PATHS
    ${DCMTK_DIR}/${dir}/include
    ${DCMTK_DIR}/${dir}
    ${DCMTK_DIR}/include/${dir}
    ${DCMTK_DIR}/include/dcmtk/${dir}
    ${DCMTK_DIR}/${dir}/include/dcmtk/${dir}
    ${DCMTK_SOURCE_DIR}/${dir}/include
    ${DCMTK_SOURCE_DIR}/${dir}
    ${DCMTK_SOURCE_DIR}/dcmtk/${dir}
    ${DCMTK_SOURCE_DIR}/include/${dir}
    ${DCMTK_SOURCE_DIR}/include/dcmtk/${dir}
    ${DCMTK_SOURCE_DIR}/${dir}/include/dcmtk/${dir}
    )
  mark_as_advanced(DCMTK_${dir}_INCLUDE_DIR)

  if(DCMTK_${dir}_INCLUDE_DIR)
    list(APPEND
      DCMTK_INCLUDE_DIRS
      ${DCMTK_${dir}_INCLUDE_DIR} ${DCMTK_SOURCE_DIR}/${dir}/include ${DCMTK_DIR}/${dir}/include)
  endif()
endforeach()

if(WIN32)
  list(APPEND DCMTK_LIBRARIES iphlpapi netapi32 wsock32 ws2_32)
endif()

if(UNIX)
  list(APPEND DCMTK_LIBRARIES pthread)
endif()


if(DCMTK_ofstd_INCLUDE_DIR)
  get_filename_component(DCMTK_dcmtk_INCLUDE_DIR
    ${DCMTK_ofstd_INCLUDE_DIR}
    PATH
    CACHE)
  list(APPEND DCMTK_INCLUDE_DIRS ${DCMTK_dcmtk_INCLUDE_DIR})
  mark_as_advanced(DCMTK_dcmtk_INCLUDE_DIR)
endif()

endif()

#-----------------------------------------------------------------------------
# RTTB part to use the found DCMTK
#   
IF(NOT DCMTK_FOUND)
  MESSAGE(SEND_ERROR "DCMTK development files not found.\n Please check variables (e.g. DCMTK_DIR) for include directories and libraries.\nYou may set environment variable DCMTK_DIR before pressing 'configure'")
ENDIF(NOT DCMTK_FOUND)

IF( NOT WIN32 )
  SET(MISSING_LIBS_REQUIRED_BY_DCMTK wrap tiff)
ENDIF( NOT WIN32 )

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${DCMTK_INCLUDE_DIRS} ${DCMTK_DIR}/include)
IF(DEFINED RTTB_USE_MITK_DCMTK)
  LIST(APPEND ALL_INCLUDE_DIRECTORIES ${DCMTK_SOURCE_DIR})
ENDIF(DEFINED RTTB_USE_MITK_DCMTK)

LIST(APPEND ALL_LIBRARIES ${DCMTK_LIBRARIES} ${MISSING_LIBS_REQUIRED_BY_DCMTK})
