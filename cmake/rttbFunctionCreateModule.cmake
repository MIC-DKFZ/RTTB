##################################################################
#
# RTTB_CREATE_MODULE
#
#! Creates a module for the automatic module dependency system within RTTB.
#!
#! Example:
#!
#! \code
#! RTTB_CREATE_MODULE2( NewModule
#!     DEPENDS PUBLIC RTTBCore
#!     PACKAGE_DEPENDS
#!       PRIVATE Boost|System
#!       PUBLIC  ITK|IO
#! \endcode
#!
#! A modules source files are specified in a separate CMake file usually
#! called files.cmake, located in the module root directory. The
#! RTTB_CREATE_MODULE2() macro evaluates the following CMake variables
#! from the files.cmake file:
#!
#! - CPP_FILES A list of .cpp files
#! - H_FILES A list of .h files without a corresponding .cpp file
#! - TXX_FILES A list of .txx files
#! - DOX_FILES A list of .dox Doxygen files
#!
#! List of variables available after the function is called:
#! - MODULE_NAME
#! - MODULE_TARGET
#! - MODULE_IS_ENABLED
#!
#! Parameters (mandatory):
#! \param <moduleName> The module name (also used as target name)
#!
#! Parameters (all optional):
#!
#! \param DEPRECATED_SINCE Marks this modules as deprecated since <arg>
#! \param DESCRIPTION A description for this module
#!
#! Multi-value Parameters (all optional):
#!
#! \param INCLUDE_DIRS Include directories for this module:
#!        \verbatim
#! [[PUBLIC|PRIVATE|INTERFACE] <dir1>...]...
#! \endverbatim
#!        The default scope for include directories is PUBLIC.
#! \param DEPENDS List of module dependencies:
#!        \verbatim
#! [[PUBLIC|PRIVATE|INTERFACE] <module1>...]...
#! \endverbatim
#!        The default scope for module dependencies is PUBLIC.
#! \param PACKAGE_DEPENDS List of public packages dependencies (e.g. Qt, VTK, etc.).
#!        Package dependencies have the following syntax:
#!        \verbatim
#! [PUBLIC|PRIVATE|INTERFACE] PACKAGE[|COMPONENT1[+COMPONENT2]...]
#! \endverbatim
#!        The default scope for package dependencies is PRIVATE.
#!
##################################################################
function(RTTB_CREATE_MODULE)

  set(_macro_params
      DEPRECATED_SINCE       # marks this modules as deprecated
      DESCRIPTION            # a description for this module
     )

  set(_macro_multiparams
      INCLUDE_DIRS           # include directories: [PUBLIC|PRIVATE|INTERFACE] <list>
      DEPENDS                # list of modules this module depends on: [PUBLIC|PRIVATE|INTERFACE] <list>
      PACKAGE_DEPENDS        # list of "packages this module depends on (e.g. Qt, VTK, etc.): [PUBLIC|PRIVATE|INTERFACE] <package-list>
      TARGET_DEPENDS         # list of CMake targets this module should depend on: [PUBLIC|PRIVATE|INTERFACE] <list>
     )

  set(_macro_options
      FORCE_STATIC           # force building this module as a static library
      HEADERS_ONLY           # this module is a headers-only library
      C_MODULE               # compile all source files as C sources
      CXX_MODULE             # compile all source files as C++ sources
     )

  cmake_parse_arguments(MODULE "${_macro_options}" "${_macro_params}" "${_macro_multiparams}" ${ARGN})

  set(MODULE_NAME ${MODULE_UNPARSED_ARGUMENTS})

  # -----------------------------------------------------------------
  # Sanity checks

  if(NOT MODULE_NAME)
    message(SEND_ERROR "The module name must not be empty")
  endif()

  set(_Module_type Module)

  set(MODULE_FILES_CMAKE files.cmake)

  # -----------------------------------------------------------------
  # Check if module should be build

  set(MODULE_TARGET ${MODULE_NAME})

  # assume worst case
  set(MODULE_IS_ENABLED 0)
  # first we check if we have an explicit module build list
  if(RTTB_MODULES_TO_BUILD)
    list(FIND RTTB_MODULES_TO_BUILD ${MODULE_NAME} _MOD_INDEX)
    if(_MOD_INDEX EQUAL -1)
      set(MODULE_IS_EXCLUDED 1)
    endif()
  endif()

  if(NOT MODULE_IS_EXCLUDED)
    message(STATUS "configuring Module ${MODULE_NAME}...") 
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

    # clear variables defined in files.cmake
    set(CPP_FILES )
    set(H_FILES )
    set(TXX_FILES )
    set(DOX_FILES )

    set(MODULE_EXPORT_DEFINE ${MODULE_NAME}_EXPORT)

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

    # ---------------------------------------------------------------
    # Create the actual module target

    if(MODULE_HEADERS_ONLY)
      add_library(${MODULE_TARGET} INTERFACE)
      # INTERFACE_LIBRARY targets may only have whitelisted properties. The property "FOLDER" is not allowed.
      # set_property(TARGET ${MODULE_TARGET} PROPERTY FOLDER "${RTTB_ROOT_FOLDER}/Modules")
    else()
      add_library(${MODULE_TARGET} ${_STATIC}
                  ${coverage_sources} ${CPP_FILES_GENERATED}
                  ${DOX_FILES})
      set_property(TARGET ${MODULE_TARGET} PROPERTY FOLDER "${RTTB_ROOT_FOLDER}/Modules")

      # add the target name to a global property which is used in the top-level
      # CMakeLists.txt file to export the target
      set_property(GLOBAL APPEND PROPERTY RTTB_MODULE_TARGETS ${MODULE_TARGET})

      if(MODULE_DEPRECATED_SINCE)
        set_property(TARGET ${MODULE_TARGET} PROPERTY RTTB_MODULE_DEPRECATED_SINCE ${MODULE_DEPRECATED_SINCE})
      endif()

      # create export macros
      CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/moduleExports.h.in ${RTTB_MODULES_CONF_DIR}/${MODULE_NAME}Exports.h @ONLY)

      # create export macros
#      generate_export_header(${MODULE_NAME}
#          EXPORT_FILE_NAME ${MODULE_NAME}Exports.h)

      target_include_directories(${MODULE_TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${RTTB_MODULES_CONF_DIR})

    endif()

    # ---------------------------------------------------------------
    # Properties for both header-only and compiled modules

    if(MODULE_HEADERS_ONLY)
      set(_module_property_type INTERFACE)
    else()
      set(_module_property_type PUBLIC)
    endif()

    if(MODULE_TARGET_DEPENDS)
      target_link_libraries(${MODULE_TARGET} ${MODULE_TARGET_DEPENDS})
    endif()

    set(DEPENDS "${MODULE_DEPENDS}")
    if(DEPENDS OR MODULE_PACKAGE_DEPENDS)
      rttb_use_modules(TARGET ${MODULE_TARGET}
                       MODULES ${DEPENDS}
                       PACKAGES ${MODULE_PACKAGE_DEPENDS}
                      )
    endif()

    # add include directories
    target_include_directories(${MODULE_TARGET} ${_module_property_type} .)
    target_include_directories(${MODULE_TARGET} ${_module_property_type} ${MODULE_INCLUDE_DIRS})

  endif()

  # -----------------------------------------------------------------
  # Record missing dependency information

  if(_MISSING_DEP)
    if(MODULE_DESCRIPTION)
      set(MODULE_DESCRIPTION "${MODULE_DESCRIPTION} (missing dependencies: ${_MISSING_DEP})")
    else()
      set(MODULE_DESCRIPTION "(missing dependencies: ${_MISSING_DEP})")
    endif()
  endif()

  set(MODULE_NAME ${MODULE_NAME} PARENT_SCOPE)
  set(MODULE_TARGET ${MODULE_TARGET} PARENT_SCOPE)
  set(MODULE_IS_ENABLED ${MODULE_IS_ENABLED} PARENT_SCOPE)

endfunction()
