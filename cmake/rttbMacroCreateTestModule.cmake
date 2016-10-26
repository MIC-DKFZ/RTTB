##################################################################
#
# RTTB_CREATE_MODULE
#
#! Creates a module for the automatic module dependency system within RTTB.
#! Configurations are generated in the moduleConf directory.
#!
#! USAGE:
#!
#! \code
#! RTTB_CREATE_MODULE( <moduleName>
#!     [INCLUDE_DIRS <include directories>]
#!     [INTERNAL_INCLUDE_DIRS <internally used include directories>]
#!     [DEPENDS <modules we need>]
#!     [PROVIDES <library name which is built>]
#!     [PACKAGE_DEPENDS <packages we need, like ITK, VTK, QT>]
#!     [HEADER_TEST]
#! \endcode
#!
#! \param MODULE_NAME_IN The name for the new module
#
##################################################################
MACRO(RTTB_CREATE_TEST_MODULE MODULE_NAME_IN)
  MACRO_PARSE_ARGUMENTS(MODULE
                        "INCLUDE_DIRS;INTERNAL_INCLUDE_DIRS;DEPENDS;DEPENDS_INTERNAL;PROVIDES;PACKAGE_DEPENDS;ADDITIONAL_LIBS"
                        "HEADER_TESTS"
                        ${ARGN})

	SET(MODULE_NAME "${MODULE_NAME_IN}Tests")
	SET(WAIT_FOR_DEPENDENCY_LIBS "Litmus") #each test depends on Litmus

	# assume worst case
	SET(MODULE_IS_ENABLED 0)
	# first we check if we have an explicit module build list
	IF(RTTB_MODULES_TO_BUILD)
		LIST(FIND RTTB_MODULES_TO_BUILD ${MODULE_NAME} _MOD_INDEX)
		IF(_MOD_INDEX EQUAL -1)
			SET(MODULE_IS_EXCLUDED 1)
		ENDIF()
	ENDIF()

	IF(NOT MODULE_IS_EXCLUDED)
		MESSAGE(STATUS "configuring Tests ${MODULE_NAME}...")
		# first of all we check for the dependencies
		RTTB_CHECK_MODULE(_MISSING_DEP ${MODULE_DEPENDS})
		IF(_MISSING_DEP)
			MESSAGE("Module ${MODULE_NAME} won't be built, missing dependency: ${_MISSING_DEP}")
			SET(MODULE_IS_ENABLED 0)
		ELSE(_MISSING_DEP)
			SET(MODULE_IS_ENABLED 1)
			# now check for every package if it is enabled. This overlaps a bit with
			# RTTB_CHECK_MODULE ...
			FOREACH(_package ${MODULE_PACKAGE_DEPENDS})
				IF((DEFINED RTTB_USE_${_package}) AND NOT (RTTB_USE_${_package}))
					MESSAGE("Module ${MODULE_NAME} won't be built. Turn on RTTB_USE_${_package} if you want to use it.")
					SET(MODULE_IS_ENABLED 0)
				ENDIF()
			ENDFOREACH()

			IF(NOT MODULE_PROVIDES)
				SET(MODULE_PROVIDES ${MODULE_NAME})
			ENDIF(NOT MODULE_PROVIDES)

			SET(DEPENDS "${MODULE_DEPENDS}")
			SET(DEPENDS_BEFORE "not initialized")
			SET(PACKAGE_DEPENDS "${MODULE_PACKAGE_DEPENDS}")
			RTTB_USE_MODULE("${MODULE_DEPENDS}")

			# ok, now create the module itself
			INCLUDE_DIRECTORIES(. ${ALL_INCLUDE_DIRECTORIES})
			INCLUDE(files.cmake)

			ORGANIZE_SOURCES(SOURCE ${CPP_FILES}
					 HEADER ${H_FILES}
					 TXX ${TXX_FILES}
					 DOC ${DOX_FILES}
					 )

			IF(ALL_LIBRARY_DIRS)
				# LINK_DIRECTORIES applies only to targets which are added after the call to LINK_DIRECTORIES
				LINK_DIRECTORIES(${ALL_LIBRARY_DIRS})
			ENDIF(ALL_LIBRARY_DIRS)

			SET(coverage_sources ${CPP_FILES} ${H_FILES} ${TXX_FILES})

			ADD_EXECUTABLE(${MODULE_PROVIDES} ${coverage_sources} ${CPP_FILES_GENERATED})

			IF(ALL_LIBRARIES)
				TARGET_LINK_LIBRARIES(${MODULE_PROVIDES} ${ALL_LIBRARIES})
			ENDIF(ALL_LIBRARIES)

			IF(MODULE_HEADER_TESTS)
				MESSAGE(STATUS "generating header tests ${MODULE_NAME}...")
				SET(HEADER_TEST_CPP "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME_IN}HeaderTests.cpp")
				MESSAGE(STATUS "generating header tests ${HEADER_TEST_CPP}")

				FILE(WRITE ${HEADER_TEST_CPP} ${RTTB_HEADER_TESTS_HEADER})
				FOREACH(_h_file ${H_FILES})
				FILE(APPEND ${HEADER_TEST_CPP} "#include <${_h_file}>\n")
				ENDFOREACH()
				FILE(APPEND ${HEADER_TEST_CPP} ${RTTB_HEADER_TESTS_FOOTER})

				ADD_EXECUTABLE(${MODULE_NAME_IN}HeaderTests ${HEADER_TEST_CPP})

				IF(ALL_LIBRARIES)
				TARGET_LINK_LIBRARIES(${MODULE_NAME_IN}HeaderTests ${ALL_LIBRARIES})
				ENDIF(ALL_LIBRARIES)

			ENDIF(MODULE_HEADER_TESTS)

			# Necessary so the build waits for libs to build
			ADD_DEPENDENCIES(${MODULE_NAME} ${WAIT_FOR_DEPENDENCY_LIBS})

			#-----------------------------------------------------------------------------

			IF(RTToolbox_BINARY_DIR)
				SET(RTToolbox_SYSTEM_INFORMATION_DIR ${RTToolbox_BINARY_DIR})
			ELSE(RTToolbox_BINARY_DIR)
				SET(RTToolbox_SYSTEM_INFORMATION_DIR ${RTTBTesting_BINARY_DIR})
			ENDIF(RTToolbox_BINARY_DIR)

			WRITE_FILE(
			 "${RTToolbox_SYSTEM_INFORMATION_DIR}/testing/HTML/TestingResults/Sites/${SITE}/${BUILDNAME}/BuildNameNotes.xml"

			 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			 "<Site BuildName=\"CMakeCache\" Name=\"crd\">\n"
			 "<BuildNameNotes>\n"
			 "<Note>\n"
			 "<DateTime>Wed Oct 24 1:00:00 EST</DateTime>\n"
			 "<Text>\n"
			 "Here is some basic information:\n"
			 "RTToolbox_SOURCE_DIR = ${RTToolbox_SOURCE_DIR}\n"
			 "RTToolbox_BINARY_DIR = ${RTToolbox_BINARY_DIR}\n"
			 "RTTBTesting_SOURCE_DIR = ${RTTBTesting_SOURCE_DIR}\n"
			 "RTTBTesting_BINARY_DIR = ${RTTBTesting_BINARY_DIR}\n"
			 "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}\n"
			 "CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}\n"
			 "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}\n"
			 "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}\n"
			 "CMAKE_SYSTEM = ${CMAKE_SYSTEM}\n"
			 "CMAKE_MAKE_PROGRAM = ${CMAKE_MAKE_PROGRAM}\n"
			 "</Text>\n"
			 "</Note>\n"
			 "</BuildNameNotes>\n"
			 "</Site>\n"
			)

		ENDIF(_MISSING_DEP)
	ENDIF(NOT MODULE_IS_EXCLUDED)
ENDMACRO(RTTB_CREATE_TEST_MODULE)

SET(RTTB_HEADER_TESTS_HEADER
"// -----------------------------------------------------------------------\n"
"// RTToolbox - DKFZ radiotherapy quantitative evaluation library\n"
"//\n"
"// (c) Copyright 2007, DKFZ, Heidelberg, Germany\n"
"// ALL RIGHTS RESERVED\n"
"//\n"
"// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.\n"
"// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR\n"
"// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED\n"
"// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.\n"
"//\n"
"//------------------------------------------------------------------------\n"
"// Automatically generated header test file \n"
"#if defined(_MSC_VER)\n"
"#pragma warning ( disable : 4786 )\n"
"#endif\n"
"#include <iostream>\n")

SET(RTTB_HEADER_TESTS_FOOTER
"\nint main ( int , char*  )\n"
"{\n"
"  return EXIT_SUCCESS;\n"
"}\n")
