##################################################################
#
# RTTB_CREATE_TEST_MODULE
#
#! Creates a module for the automatic module dependency system within RTTB.
#! Configurations are generated in the moduleConf directory.
#!
#! USAGE:
#!
#! \code
#! RTTB_CREATE_TEST_MODULE( <moduleName>
#!     [INCLUDE_DIRS <include directories>]
#!     [DEPENDS <modules we need>]
#!     [PACKAGE_DEPENDS <packages we need, like ITK, VTK, QT>]
#!     [HEADER_TEST]
#! \endcode
#!
#! \param MODULE_NAME_IN The name for the new module
#
##################################################################
MACRO(RTTB_CREATE_TEST_MODULE MODULE_NAME_IN)

  set(_macro_params)

  set(_macro_multiparams
      INCLUDE_DIRS           # include directories: [PUBLIC|PRIVATE|INTERFACE] <list>
      DEPENDS                # list of modules this module depends on: [PUBLIC|PRIVATE|INTERFACE] <list>
      PACKAGE_DEPENDS        # list of "packages this module depends on (e.g. Qt, VTK, etc.): [PUBLIC|PRIVATE|INTERFACE] <package-list>
     )

  set(_macro_options
      HEADER_TESTS
     )

  cmake_parse_arguments(MODULE "${_macro_options}" "${_macro_params}" "${_macro_multiparams}" ${ARGN})

  SET(MODULE_NAME "${RTToolbox_PREFIX}${MODULE_NAME_IN}Tests")
  SET(WAIT_FOR_DEPENDENCY_LIBS "Litmus") #each test depends on Litmus
  set(MODULE_FILES_CMAKE files.cmake)

  # -----------------------------------------------------------------
  # Check if module should be build

  set(MODULE_TARGET ${MODULE_NAME})

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
    _rttb_parse_package_args(${MODULE_PACKAGE_DEPENDS})
    rttb_check_module_dependencies(MODULES ${MODULE_DEPENDS}
                                   PACKAGES ${PACKAGE_NAMES}
                                   MISSING_DEPENDENCIES_VAR _MISSING_DEP
                                   PACKAGE_DEPENDENCIES_VAR PACKAGE_NAMES)

    if(_MISSING_DEP)
      message("${_Module_type} ${MODULE_NAME} won't be built, missing dependency: ${_MISSING_DEP}")
      set(MODULE_IS_ENABLED 0)
    else()
      set(MODULE_IS_ENABLED 1)
      # now check for every package if it is enabled. This overlaps a bit with
      # RTTB_CHECK_MODULE ...
      foreach(_package ${PACKAGE_NAMES})
        if((DEFINED RTTB_USE_${_package}) AND NOT (RTTB_USE_${_package}))
          message("${_Module_type} ${MODULE_NAME} won't be built. Turn on RTTB_USE_${_package} if you want to use it.")
          set(MODULE_IS_ENABLED 0)
          break()
        endif()
      endforeach()
    endif()
  endif()

  # -----------------------------------------------------------------
  # Start creating the module

  if(MODULE_IS_ENABLED)

    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FILES_CMAKE}")
      include(${MODULE_FILES_CMAKE})
    endif()

    if(CPP_FILES)
      set(MODULE_HEADERS_ONLY 0)
    else()
      set(MODULE_HEADERS_ONLY 1)
    endif()

    if(MODULE_FORCE_STATIC)
      set(_STATIC ${RTTB_WIN32_FORCE_STATIC)
    else()
      set(_STATIC )
    endif(MODULE_FORCE_STATIC)

    ORGANIZE_SOURCES(
      SOURCE ${CPP_FILES}
      HEADER ${H_FILES}
      TXX ${TXX_FILES}
      DOC ${DOX_FILES}
      )

    set(coverage_sources
        ${CPP_FILES} ${H_FILES} ${GLOBBED__H_FILES} ${CORRESPONDING__H_FILES} ${TXX_FILES})

    ADD_EXECUTABLE(${MODULE_TARGET} ${coverage_sources} ${CPP_FILES_GENERATED})

    SET(DEPENDS "${MODULE_DEPENDS}")
    SET(DEPENDS_BEFORE "not initialized")

    rttb_use_modules(TARGET ${MODULE_TARGET}
                     MODULES ${DEPENDS}
                     PACKAGES ${MODULE_PACKAGE_DEPENDS}
                    )

    # add include directories
    target_include_directories(${MODULE_TARGET} PUBLIC .)
    target_include_directories(${MODULE_TARGET} PUBLIC ${MODULE_INCLUDE_DIRS})

    IF(MODULE_HEADER_TESTS)
      MESSAGE(STATUS "generating header tests ${MODULE_NAME}...")
      SET(HEADER_TEST_CPP "${CMAKE_CURRENT_BINARY_DIR}/${RTToolbox_PREFIX}${MODULE_NAME_IN}HeaderTest.cpp")
      MESSAGE(STATUS "generating header tests ${HEADER_TEST_CPP}")

	  FILE(WRITE ${HEADER_TEST_CPP} ${RTTB_HEADER_TESTS_HEADER})
	  FOREACH(_h_file ${H_FILES})
	    FILE(APPEND ${HEADER_TEST_CPP} "#include <${_h_file}>\n")
	  ENDFOREACH()
	  FILE(APPEND ${HEADER_TEST_CPP} ${RTTB_HEADER_TESTS_FOOTER})

	  ADD_EXECUTABLE(${RTToolbox_PREFIX}${MODULE_NAME_IN}HeaderTest ${HEADER_TEST_CPP})

      SET(DEPENDS "${MODULE_DEPENDS}")
      SET(DEPENDS_BEFORE "not initialized")

      rttb_use_modules(TARGET ${RTToolbox_PREFIX}${MODULE_NAME_IN}HeaderTest
                       MODULES ${DEPENDS}
                       PACKAGES ${MODULE_PACKAGE_DEPENDS}
                      )

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

    endif()
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
