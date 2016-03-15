#-----------------------------------------------------------------------------
# This is the root RTToolbox CMakeList file.
#-----------------------------------------------------------------------------
PROJECT(RTToolbox)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11.2)
IF(CMAKE_BACKWARDS_COMPATIBILITY GREATER 2.8.11.2)
  SET(CMAKE_BACKWARDS_COMPATIBILITY 2.8.11.2 CACHE STRING "Latest version of CMake when this project was released." FORCE)
ENDIF(CMAKE_BACKWARDS_COMPATIBILITY GREATER 2.8.11.2)

# RTToolbox version number.
SET(RTToolbox_VERSION_MAJOR "4")
SET(RTToolbox_VERSION_MINOR "1")
SET(RTToolbox_VERSION_PATCH "1")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolbox.
SET(RTToolbox_VERSION_STRING "${RTToolbox_VERSION_MAJOR}.${RTToolbox_VERSION_MINOR}")
SET(RTToolbox_FULL_VERSION_STRING "${RTToolbox_VERSION_MAJOR}.${RTToolbox_VERSION_MINOR}.${RTToolbox_VERSION_PATCH}")

# default build type
SET(CMAKE_BUILD_TYPE Release)

MARK_AS_ADVANCED(BUILD_SHARED_LIBS)

IF (WIN32)
	IF (MSVC_VERSION LESS 1600)
		MESSAGE(FATAL_ERROR "RTToolbox requires at least Visual Studio 2010.")
	ENDIF(MSVC_VERSION LESS 1600)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
ELSE (WIN32)
	IF (CMAKE_COMPILER_IS_GNUCC)
		IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 5.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0)
			MESSAGE(AUTHOR_WARNING "RTToolbox was only tested with GCC 4.6 and GCC 4.9. You are using GCC " ${CMAKE_CXX_COMPILER_VERSION} ". This compiler version might not work.")
		ENDIF()
	ENDIF()
ENDIF(WIN32)


IF (NOT (CMAKE_MAJOR_VERSION LESS 3))
	IF(COMMAND CMAKE_POLICY)
	  # Enable old CMake behaviour when dealing with export_library_dependencies().
	  # This is necessary to avoid warnings in CMake versions
	  # greater than 3.0
	  # See http://www.cmake.org/cmake/help/v3.0/policy/CMP0033.html
	  CMAKE_POLICY(SET CMP0033 OLD)
	ENDIF(COMMAND CMAKE_POLICY)
ENDIF()

#-----------------------------------------------------------------------------
# CMake Function(s) and Macro(s)
#-----------------------------------------------------------------------------

include(cmake/MacroParseArguments.cmake)
include(cmake/rttbMacroCreateModuleConf.cmake)
include(cmake/rttbMacroCreateModule.cmake)
include(cmake/rttbMacroCreateApplication.cmake)
include(cmake/rttbMacroCheckModule.cmake)
include(cmake/rttbMacroUseModule.cmake)
include(cmake/rttbMacroCreateTestModule.cmake)
include(cmake/rttbFunctionOrganizeSources.cmake)
include(cmake/rttbMacroCreateApplicationTests.cmake)

#-----------------------------------------------------------------------------
# Basis config RTTB module infrastructure
#-----------------------------------------------------------------------------

set(RTTB_MODULES_CONF_DIR ${RTToolbox_BINARY_DIR}/modulesConf CACHE INTERNAL "Modules Conf")
set(RTTB_MODULES_PACKAGE_DEPENDS_DIR ${RTToolbox_SOURCE_DIR}/cmake/PackageDepends)
set(MODULES_PACKAGE_DEPENDS_DIRS ${RTTB_MODULES_PACKAGE_DEPENDS_DIR})

#-----------------------------------------------------------------------------
# Testing setup
# Configure Dart testing support.  This should be done before any
# MESSAGE(FATAL_ERROR ...) commands are invoked.
#-----------------------------------------------------------------------------
SET(CTEST_NEW_FORMAT 1)
INCLUDE(CTest)
ENABLE_TESTING()

IF(BUILD_TESTING)
  CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/RemoveTemporaryFiles.cmake.in
    ${RTToolbox_BINARY_DIR}/cmake/RemoveTemporaryFiles.cmake @ONLY IMMEDIATE)
  CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/rttbSampleBuildTest.cmake.in
    ${RTToolbox_BINARY_DIR}/cmake/rttbSampleBuildTest.cmake @ONLY)
  CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/CTestCustom.ctest.in
    ${RTToolbox_BINARY_DIR}/cmake/CTestCustom.ctest @ONLY)
  FILE(WRITE ${RTToolbox_BINARY_DIR}/CTestCustom.cmake
    "INCLUDE(\"${RTToolbox_BINARY_DIR}/cmake/CTestCustom.ctest\")\n")

  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

#-----------------------------------------------------------------------------
# Output directories.
#-----------------------------------------------------------------------------

IF(NOT LIBRARY_OUTPUT_PATH)
  SET (LIBRARY_OUTPUT_PATH ${RTToolbox_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)
IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET (EXECUTABLE_OUTPUT_PATH ${RTToolbox_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)


MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
SET(RTToolbox_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(RTToolbox_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")

#-----------------------------------------------------------------------------
# Find Doxygen.
#-----------------------------------------------------------------------------
FIND_PROGRAM(DOXYGEN_EXECUTABLE "doxygen")

#-----------------------------------------------------------------------------
# Installation vars.
# RTToolbox_INSTALL_BIN_DIR          - binary dir (executables)
# RTToolbox_INSTALL_LIB_DIR          - library dir (libs)
# RTToolbox_INSTALL_INCLUDE_DIR      - include dir (headers)
# RTToolbox_INSTALL_NO_DEVELOPMENT   - do not install development files
# RTToolbox_INSTALL_NO_RUNTIME       - do not install runtime files
# RTToolbox_INSTALL_NO_DOCUMENTATION - do not install documentation files
# Remark: needs directory are stored with no leading slash (CMake 2.4 and newer)
#-----------------------------------------------------------------------------

IF(NOT RTTOOLBOX_INSTALL_BIN_DIR)
  SET(RTTOOLBOX_INSTALL_BIN_DIR "bin")
ENDIF(NOT RTTOOLBOX_INSTALL_BIN_DIR)

IF(NOT RTTOOLBOX_INSTALL_LIB_DIR)
  SET(RTTOOLBOX_INSTALL_LIB_DIR "lib")
ENDIF(NOT RTTOOLBOX_INSTALL_LIB_DIR)

IF(NOT RTTOOLBOX_INSTALL_PACKAGE_DIR)
  SET(RTTOOLBOX_INSTALL_PACKAGE_DIR "lib")
ENDIF(NOT RTTOOLBOX_INSTALL_PACKAGE_DIR)

IF(NOT RTTOOLBOX_INSTALL_INCLUDE_DIR)
  SET(RTTOOLBOX_INSTALL_INCLUDE_DIR "include")
ENDIF(NOT RTTOOLBOX_INSTALL_INCLUDE_DIR)

IF(NOT RTTOOLBOX_INSTALL_NO_DEVELOPMENT)
  SET(RTTOOLBOX_INSTALL_NO_DEVELOPMENT 0)
ENDIF(NOT RTTOOLBOX_INSTALL_NO_DEVELOPMENT)

IF(NOT RTTOOLBOX_INSTALL_NO_RUNTIME)
  SET(RTTOOLBOX_INSTALL_NO_RUNTIME 0)
ENDIF(NOT RTTOOLBOX_INSTALL_NO_RUNTIME)

IF(NOT RTTOOLBOX_INSTALL_NO_DOCUMENTATION)
  SET(RTTOOLBOX_INSTALL_NO_DOCUMENTATION 0)
ENDIF(NOT RTTOOLBOX_INSTALL_NO_DOCUMENTATION)

SET(RTTOOLBOX_INSTALL_NO_LIBRARIES)
IF(RTTOOLBOX_BUILD_SHARED_LIBS)
  IF(RTTOOLBOX_INSTALL_NO_RUNTIME AND RTTOOLBOX_INSTALL_NO_DEVELOPMENT)
    SET(RTTOOLBOX_INSTALL_NO_LIBRARIES 1)
  ENDIF(RTTOOLBOX_INSTALL_NO_RUNTIME AND RTTOOLBOX_INSTALL_NO_DEVELOPMENT)
ELSE(RTTOOLBOX_BUILD_SHARED_LIBS)
  IF(RTTOOLBOX_INSTALL_NO_DEVELOPMENT)
    SET(RTTOOLBOX_INSTALL_NO_LIBRARIES 1)
  ENDIF(RTTOOLBOX_INSTALL_NO_DEVELOPMENT)
ENDIF(RTTOOLBOX_BUILD_SHARED_LIBS)

# set RTToolbox_DIR so it can be used by subprojects
SET(RTToolbox_DIR "${CMAKE_BINARY_DIR}" CACHE INTERNAL "RTToolbox dir to be used by subprojects")

#-----------------------------------------------------------------------------
# DCMTK MT-Flag treat
#-----------------------------------------------------------------------------
option(RTTB_DCMTK_COMPLIANCE_ENFORCE_MT "This enforces the whole RTToolbox to be compiled with /MT,/MTd to be compliant with DCMTK" OFF)

string(FIND ${CMAKE_GENERATOR} "Visual Studio" RTTB_VS_USED)

if(RTTB_DCMTK_COMPLIANCE_ENFORCE_MT AND RTTB_VS_USED EQUAL 0)
  message(STATUS "Enforce DCMTK compliance: /MT and /MTd flags are used")

  string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  message(STATUS "CMAKE_C_FLAGS_DEBUG set to: ${CMAKE_C_FLAGS_DEBUG}")
  string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  message(STATUS "CMAKE_C_FLAGS_RELEASE set to: ${CMAKE_C_FLAGS_RELEASE}")
  string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS_MINSIZEREL})
  message(STATUS "CMAKE_C_FLAGS_MINSIZEREL set to: ${CMAKE_C_FLAGS_MINSIZEREL}")
  string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO})
  message(STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO set to: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")

  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  message(STATUS "CMAKE_CXX_FLAGS_DEBUG set to: ${CMAKE_CXX_FLAGS_DEBUG}")
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  message(STATUS "CMAKE_CXX_FLAGS_RELEASE set to: ${CMAKE_CXX_FLAGS_RELEASE}")
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL})
  message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL set to: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
  message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO set to: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")  
endif()

#-----------------------------------------------------------------------------
# Advanced RTToolbox configuration
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# RTToolbox build configuration options.
IF (WIN32)
  OPTION(BUILD_SHARED_LIBS "Build RTToolbox with shared libraries." OFF)
ELSE (WIN32)
  OPTION(BUILD_SHARED_LIBS "Build RTToolbox with shared libraries." ON)
ENDIF (WIN32)

IF(BUILD_SHARED_LIBS)
  IF(WIN32)
  MESSAGE(FATAL_ERROR "RTToolbox currently does not support a dynamic build on Windows. We are working on that...")
  ENDIF(WIN32)
ELSE(BUILD_SHARED_LIBS)
  IF(UNIX)
  MESSAGE(FATAL_ERROR "RTToolbox currently does not support a static build on unix like systems. We are working on that...")
  ENDIF(UNIX)
ENDIF(BUILD_SHARED_LIBS)

SET(RTToolbox_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

IF(NOT RTToolbox_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of RTToolbox.
  SET(RTToolbox_LIBRARY_PROPERTIES 
      VERSION   "${RTToolbox_VERSION_MAJOR}.${RTToolbox_VERSION_MINOR}.${RTToolbox_VERSION_PATCH}"
      SOVERSION "${RTToolbox_VERSION_MAJOR}.${RTToolbox_VERSION_MINOR}"
     )
ENDIF(NOT RTToolbox_NO_LIBRARY_VERSION)

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
# 
#-----------------------------------------------------------------------------

CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/RTToolboxConfigure.h.in 
  ${RTToolbox_BINARY_DIR}/RTToolboxConfigure.h)
  
IF(NOT RTTOOLBOX_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${RTToolbox_BINARY_DIR}/RTToolboxConfigure.h
    DESTINATION ${RTTOOLBOX_INSTALL_INCLUDE_DIR}
    COMPONENT Development)
ENDIF(NOT RTTOOLBOX_INSTALL_NO_DEVELOPMENT)

#-----------------------------------------------------------------------------
# The entire RTToolbox tree should use the same include path
#-----------------------------------------------------------------------------

#Default include dir. Others dirs will be defined by activated subprojects
INCLUDE_DIRECTORIES(${RTToolbox_BINARY_DIR})

LINK_DIRECTORIES(${LIBARY_OUTPUT_PATH})

#Prepare the correct target information export by the subprojects
SET(RTToolbox_TARGETS_FILE "${RTToolbox_BINARY_DIR}/RTToolboxTargets.cmake")
FILE(WRITE ${RTToolbox_TARGETS_FILE} "# Generated by CMake, do not edit!")

#-----------------------------------------------------------------------------
# Dispatch the build into the proper subdirectories.
#-----------------------------------------------------------------------------

MESSAGE (STATUS "generating Project RTToolbox")
ADD_SUBDIRECTORY (code)
ADD_SUBDIRECTORY (demoapps)

IF (BUILD_TESTING)
  ADD_SUBDIRECTORY (testing)
ENDIF (BUILD_TESTING)

ADD_SUBDIRECTORY (documentation)

#-----------------------------------------------------------------------------
# Help other projects use RTToolbox.
#-----------------------------------------------------------------------------

EXPORT(PACKAGE RTToolbox)

# Copy the UseRTToolbox.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/UseRTToolbox.cmake.in
               ${RTToolbox_BINARY_DIR}/UseRTToolbox.cmake COPYONLY IMMEDIATE)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${RTToolbox_BINARY_DIR}/RTToolboxLibraryDepends.cmake)

# Create the RTToolboxConfig.cmake file containing the RTToolbox configuration.
INCLUDE (${RTToolbox_SOURCE_DIR}/rttbGenerateRTToolboxConfig.cmake)

IF(NOT RTToolbox_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES
    ${RTToolbox_BINARY_DIR}/RTToolboxConfig.cmake
    ${RTToolbox_BINARY_DIR}/RTToolboxTargets.cmake
    ${RTToolbox_BINARY_DIR}/RTToolboxLibraryDepends.cmake
    ${RTToolbox_BINARY_DIR}/UseRTToolbox.cmake
    DESTINATION ${RTTOOLBOX_INSTALL_PACKAGE_DIR}
    COMPONENT Development
    )
ENDIF(NOT RTToolbox_INSTALL_NO_DEVELOPMENT)
