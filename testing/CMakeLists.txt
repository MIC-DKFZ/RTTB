MESSAGE(STATUS "processing RTToolbox testing code")

# Testing branch
PROJECT(RTTBTesting)

#-----------------------------------------------------------------------------
# extract and build Litmus
#-----------------------------------------------------------------------------

include(ExternalProject)
message(STATUS "Litmus will be automatically downloaded and built.")
set(LITMUS_SOURCE_DIR "${CMAKE_BINARY_DIR}/external/Litmus-src")
set(LITMUS_BUILD_DIR "${CMAKE_BINARY_DIR}/external/Litmus-build")
set(LITMUS_CMAKE_DIR "${CMAKE_BINARY_DIR}/external/Litmus-cmake")

IF (BUILD_Tester_All OR (BUILD_Tester_Interpolation AND BUILD_InterpolationMatchPointTransformation) OR (BUILD_Tester_IO AND BUILD_IO_ITK) OR (BUILD_Tester_Apps))
	set(ENABLE_ITK "-DLIT_ENABLE_ITK_SUPPORT:BOOL=ON")
	set(ITK_DIRECTORY "-DITK_DIR:PATH=${ITK_DIR}")
	IF (RTTB_USE_SYSTEM_HDF5)
		set(SYSTEM_HDF5 "-DLIT_USE_SYSTEM_HDF5:BOOL=ON")
		set(LITMUS_HDF5_DIR "-DHDF5_DIR:PATH=${HDF5_DIR}")
	ENDIF()
ENDIF()
#extract and build Litmus
ExternalProject_Add(
	Litmus
	URL ${RTToolbox_SOURCE_DIR}/utilities/Litmus/Litmus.tar.gz
	URL_HASH SHA1=73CE5302C35D984090B70B4A44644DA916A5E0A3
	SOURCE_DIR ${LITMUS_SOURCE_DIR}
	BINARY_DIR ${LITMUS_BUILD_DIR}
	PREFIX ${LITMUS_CMAKE_DIR}
	INSTALL_COMMAND ""
	UPDATE_COMMAND "" # Don't update SVN on every build
	CMAKE_ARGS 
		-DBUILD_TESTING:BOOL=OFF
		-DCMAKE_CXX_STANDARD=11
		${ENABLE_ITK}
		${ITK_DIRECTORY}
		${SYSTEM_HDF5}
		${LITMUS_HDF5_DIR}
)

set(RTTBDATA_DIR "${CMAKE_BINARY_DIR}/external/RTTBData")
set(TEST_DATA_ROOT ${RTTBDATA_DIR})

#download RTTB data
message(STATUS "RTTBdata will be automatically downloaded.")
ExternalProject_Add(
	RTTBData
    SOURCE_DIR ${RTTBDATA_DIR}
    GIT_REPOSITORY "https://phabricator.mitk.org/source/rttb-data.git"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
  )

#-----------------------------------------------------------------------------
# Configure Testing branch
#-----------------------------------------------------------------------------
MAKE_DIRECTORY(${RTTBTesting_BINARY_DIR}/Temporary)

OPTION(BUILD_Tester_All "All testing modules will be built" OFF)

MESSAGE(STATUS "Process All Tests...")

#-----------------------------------------------------------------------------
# Include sub directories
#-----------------------------------------------------------------------------
OPTION(BUILD_Tester_Core "build project on/off" OFF)
OPTION(BUILD_Tester_Examples "build project on/off" OFF)
OPTION(BUILD_Tester_Algorithms "build project on/off" OFF)
OPTION(BUILD_Tester_Models "build project on/off" OFF)
OPTION(BUILD_Tester_IO "build project on/off" OFF)
OPTION(BUILD_Tester_Masks "build project on/off" OFF)
OPTION(BUILD_Tester_Interpolation "build project on/off" OFF)
OPTION(BUILD_Tester_Apps "build project on/off" OFF)
OPTION(BUILD_Tester_Validation "build project on/off" OFF)

IF(BUILD_Tester_All OR BUILD_Tester_Core)
	ADD_SUBDIRECTORY(core)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_Examples)
	ADD_SUBDIRECTORY(examples)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_Algorithms)
	ADD_SUBDIRECTORY(algorithms)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_Models)
	ADD_SUBDIRECTORY(models)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_IO)
	ADD_SUBDIRECTORY(io)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_Masks)
	ADD_SUBDIRECTORY(masks)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_Interpolation)
	ADD_SUBDIRECTORY(interpolation)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_Validation)
	ADD_SUBDIRECTORY(validation)
ENDIF()

IF(BUILD_Tester_All OR BUILD_Tester_Apps)
	ADD_SUBDIRECTORY(apps)
ENDIF()
