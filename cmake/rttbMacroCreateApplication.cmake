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
  MACRO_PARSE_ARGUMENTS(APP
                        "INCLUDE_DIRS;DEPENDS;PACKAGE_DEPENDS"
                        "FORCE_STATIC;HEADERS_ONLY"
                        ${ARGN})

	SET(APP_NAME ${APP_NAME_IN})
	SET(WAIT_FOR_DEPENDENCY_LIBS "ArgumentParsingLib") #each application depends on ArgumentParsingLib

    MESSAGE(STATUS "configuring Application ${APP_NAME}...")
    # first of all we check for the dependencies
    RTTB_CHECK_MODULE(_MISSING_DEP ${APP_DEPENDS})
    IF(_MISSING_DEP)
      MESSAGE("Application ${APP_NAME} won't be built, missing dependency: ${_MISSING_DEP}")
      SET(APP_IS_ENABLED 0)
    ELSE(_MISSING_DEP)
      SET(APP_IS_ENABLED 1)
      # now check for every package if it is enabled. This overlaps a bit with
      # RTTB_CHECK_MODULE ...
      FOREACH(_package ${APP_PACKAGE_DEPENDS})
        IF((DEFINED RTTB_USE_${_package}) AND NOT (RTTB_USE_${_package}))
          MESSAGE("Application ${APP_NAME} won't be built. Turn on RTTB_USE_${_package} if you want to use it.")
		  SET(APP_IS_ENABLED 0)
	    ENDIF()
      ENDFOREACH()

      IF(APP_IS_ENABLED)
		SET(DEPENDS "${APP_DEPENDS}")
		SET(DEPENDS_BEFORE "not initialized")
		SET(PACKAGE_DEPENDS "${APP_PACKAGE_DEPENDS}")
		RTTB_USE_MODULE("${APP_DEPENDS}")

		IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/source)
          SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/source)
        ENDIF()

        IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include)
          SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/include)
        ENDIF()

		INCLUDE_DIRECTORIES(. ${ALL_INCLUDE_DIRECTORIES})
		INCLUDE(files.cmake)

		ORGANIZE_SOURCES(SOURCE ${CPP_FILES}
			     HEADER ${H_FILES}
			     TPP ${TPP_FILES}
			     DOC ${DOX_FILES}
			     )

		LINK_DIRECTORIES(${ALL_LIBRARY_DIRS})

	    SET(coverage_sources ${CPP_FILES} ${H_FILES} ${TXX_FILES})

		ADD_EXECUTABLE(${APP_NAME} ${coverage_sources})

        TARGET_LINK_LIBRARIES(${APP_NAME} ${ALL_LIBRARIES})
		# Necessary so the build waits for libs to build
		ADD_DEPENDENCIES(${APP_NAME} ${WAIT_FOR_DEPENDENCY_LIBS})
      ENDIF(APP_IS_ENABLED)

    ENDIF(_MISSING_DEP)

ENDMACRO(RTTB_CREATE_APPLICATION)
