MESSAGE(STATUS "processing RTToolbox testing code")

# Testing branch
PROJECT(RTTBTesting)

#-----------------------------------------------------------------------------
# Find Litmus.
#-----------------------------------------------------------------------------
MESSAGE(STATUS "searching Litmus")
FIND_PACKAGE(Litmus)
IF(Litmus_FOUND)
INCLUDE(${Litmus_USE_FILE})
SET(ADDITIONAL_TEST_INCLUDES ${Litmus_USE_FILE})
ELSE(Litmus_FOUND)
MESSAGE(FATAL_ERROR
		"Cannot build without Litmus.  Please set Litmus_DIR.")
ENDIF(Litmus_FOUND)

#-----------------------------------------------------------------------------
# Configure Testing branch
#-----------------------------------------------------------------------------
MAKE_DIRECTORY(${RTTBTesting_BINARY_DIR}/temporary)


MESSAGE(STATUS "Process All Tests...")

#-----------------------------------------------------------------------------
# Include sub directories
#-----------------------------------------------------------------------------
OPTION(BUILD_RTToolbox_core_Tester "build project on/off" OFF)
OPTION(BUILD_RTToolbox_Test_examples "build project on/off" OFF)
OPTION(BUILD_RTToolbox_algorithms_Tester "build project on/off" OFF)
OPTION(BUILD_RTToolbox_models_Tester "build project on/off" OFF)
OPTION(BUILD_RTToolbox_io_Tester "build project on/off" OFF)
OPTION(BUILD_RTToolbox_masks_Tester "build project on/off" OFF)
OPTION(BUILD_RTToolbox_interpolation_Tester "build project on/off" OFF)
OPTION(BUILD_RTToolbox_demoapps_Tester "build project on/off" OFF)

IF(${BUILD_RTToolbox_core_Tester} STREQUAL ON)
ADD_SUBDIRECTORY(core)
ENDIF(${BUILD_RTToolbox_core_Tester} STREQUAL ON)

IF(${BUILD_RTToolbox_Test_examples} STREQUAL ON)
ADD_SUBDIRECTORY(examples)
ENDIF(${BUILD_RTToolbox_Test_examples} STREQUAL ON)

IF(${BUILD_RTToolbox_algorithms_Tester} STREQUAL ON)
ADD_SUBDIRECTORY(algorithms)
ENDIF(${BUILD_RTToolbox_algorithms_Tester} STREQUAL ON)

IF(${BUILD_RTToolbox_models_Tester} STREQUAL ON)
ADD_SUBDIRECTORY(models)
ENDIF(${BUILD_RTToolbox_models_Tester} STREQUAL ON)

IF(${BUILD_RTToolbox_io_Tester} STREQUAL ON)
ADD_SUBDIRECTORY(io)
ENDIF(${BUILD_RTToolbox_io_Tester} STREQUAL ON)

IF(${BUILD_RTToolbox_masks_Tester} STREQUAL ON)
ADD_SUBDIRECTORY(masks)
ENDIF(${BUILD_RTToolbox_masks_Tester} STREQUAL ON)

IF(${BUILD_RTToolbox_interpolation_Tester} STREQUAL ON)
ADD_SUBDIRECTORY(interpolation)
ENDIF(${BUILD_RTToolbox_interpolation_Tester} STREQUAL ON)

IF(${BUILD_RTToolbox_demoapps_Tester} STREQUAL ON)
ADD_SUBDIRECTORY(demoapps)
ENDIF(${BUILD_RTToolbox_demoapps_Tester} STREQUAL ON)

