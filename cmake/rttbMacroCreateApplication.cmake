##################################################################
#
# RTTB_CREATE_APPLICATION
#
#! Creates a application using the automatic module dependency system within RTTB.
#! Configurations are generated in the moduleConf directory.
#!
#! USAGE:
#!
#! \code
#! MAP_CREATE_APPLICATION( <appName>
#!     [INCLUDE_DIRS <include directories>]
#!     [DEPENDS <modules we need>]
#!     [PACKAGE_DEPENDS <packages we need, like ITK, Litmus>]
#! \endcode
#!
#! \param APP_NAME_IN The name for the new application
#
##################################################################
MACRO(RTTB_CREATE_APPLICATION APP_NAME_IN)

  set(_macro_params)

  set(_macro_multiparams
      INCLUDE_DIRS           # include directories: [PUBLIC|PRIVATE|INTERFACE] <list>
      DEPENDS                # list of modules this module depends on: [PUBLIC|PRIVATE|INTERFACE] <list>
      PACKAGE_DEPENDS        # list of "packages this module depends on (e.g. Qt, VTK, etc.): [PUBLIC|PRIVATE|INTERFACE] <package-list>
     )

  set(_macro_options)

  cmake_parse_arguments(APP "${_macro_options}" "${_macro_params}" "${_macro_multiparams}" ${ARGN})

	SET(APP_NAME ${APP_NAME_IN})
	SET(WAIT_FOR_DEPENDENCY_LIBS "ArgumentParsingLib") #each application depends on ArgumentParsingLib
	set(MODULE_FILES_CMAKE files.cmake)
    # assume worst case
    SET(APP_IS_ENABLED 0)

    MESSAGE(STATUS "configuring Application ${APP_NAME}...")
    # first of all we check for the dependencies


    _rttb_parse_package_args(${APP_DEPENDS})
    rttb_check_module_dependencies(MODULES ${MODULE_DEPENDS}
                                   PACKAGES ${PACKAGE_NAMES}
                                   MISSING_DEPENDENCIES_VAR _MISSING_DEP
                                   PACKAGE_DEPENDENCIES_VAR PACKAGE_NAMES)

    if(_MISSING_DEP)
      MESSAGE("Application ${APP_NAME} won't be built, missing dependency: ${_MISSING_DEP}")
      SET(APP_IS_ENABLED 0)
    else()
      SET(APP_IS_ENABLED 1)
      # now check for every package if it is enabled. This overlaps a bit with
      # RTTB_CHECK_MODULE ...
      foreach(_package ${PACKAGE_NAMES})
        if((DEFINED RTTB_USE_${_package}) AND NOT (RTTB_USE_${_package}))
          message("Application ${APP_NAME} won't be built. Turn on RTTB_USE_${_package} if you want to use it.")
          set(MODULE_IS_ENABLED 0)
          break()
        endif()
      endforeach()
    endif()


  IF(APP_IS_ENABLED)

    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_FILES_CMAKE}")
      include(${MODULE_FILES_CMAKE})
    endif()

	ORGANIZE_SOURCES(SOURCE ${CPP_FILES}
			 HEADER ${H_FILES}
			 TPP ${TPP_FILES}
			 DOC ${DOX_FILES}
			 )

	SET(coverage_sources ${CPP_FILES} ${H_FILES} ${TXX_FILES})

	ADD_EXECUTABLE(${APP_NAME} ${coverage_sources})

    SET(DEPENDS "${APP_DEPENDS}")
	SET(DEPENDS_BEFORE "not initialized")
    rttb_use_modules(TARGET ${APP_NAME}
                     MODULES ${DEPENDS}
                     PACKAGES ${APP_PACKAGE_DEPENDS}
                    )

    set (_app_include_dirs . ${APP_INCLUDE_DIRS})
	IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/source)
	  SET(_app_include_dirs ${_app_include_dirs} ${CMAKE_CURRENT_SOURCE_DIR}/source)
	ENDIF()
	IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include)
	  SET(_app_include_dirs ${_app_include_dirs} ${CMAKE_CURRENT_SOURCE_DIR}/include)
	ENDIF()
    target_include_directories(${APP_NAME} PUBLIC ${_app_include_dirs})

    INSTALL(TARGETS ${APP_NAME} EXPORT RTToolboxExport
       RUNTIME DESTINATION ${RTTOOLBOX_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
       LIBRARY DESTINATION ${RTTOOLBOX_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
       ARCHIVE DESTINATION ${RTTOOLBOX_INSTALL_LIB_DIR} COMPONENT Development)

  ENDIF(APP_IS_ENABLED)

ENDMACRO(RTTB_CREATE_APPLICATION)
