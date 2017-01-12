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

IF ((BUILD_Tester_Interpolation AND BUILD_InterpolationMatchPointTransformation) OR (BUILD_Tester_IO AND BUILD_IO_ITK) OR (BUILD_Tester_Apps AND BUILD_App_DoseTool))
	set(ENABLE_ITK "-DLIT_ENABLE_ITK_SUPPORT:BOOL=ON")
	set(ITK_DIRECTORY "-DITK_DIR:STRING=${ITK_DIR}")
ENDIF()
#extract and build Litmus
ExternalProject_Add(
	Litmus
	URL ${RTToolbox_SOURCE_DIR}/utilities/Litmus/Litmus.tar.gz
	SOURCE_DIR ${LITMUS_SOURCE_DIR}
	BINARY_DIR ${LITMUS_BUILD_DIR}
	PREFIX ${LITMUS_CMAKE_DIR}
	INSTALL_COMMAND ""
	UPDATE_COMMAND "" # Don't update SVN on every build
	CMAKE_ARGS 
		-DBUILD_TESTING:BOOL=OFF
		${ENABLE_ITK}
		${ITK_DIRECTORY}
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
	IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
		ADD_SUBDIRECTORY(examples_virtuos)
	ENDIF()
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
