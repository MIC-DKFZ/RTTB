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
#!     [EXPORT_DEFINE <declspec macro name for dll exports>]
#! \endcode
#!
#! \param MODULE_NAME_IN The name for the new module
#
##################################################################
MACRO(RTTB_CREATE_MODULE MODULE_NAME_IN)
  MACRO_PARSE_ARGUMENTS(MODULE
                        "INCLUDE_DIRS;INTERNAL_INCLUDE_DIRS;DEPENDS;DEPENDS_INTERNAL;PROVIDES;PACKAGE_DEPENDS;EXPORT_DEFINE;ADDITIONAL_LIBS"
                        "FORCE_STATIC;HEADERS_ONLY"
                        ${ARGN})
                        
  SET(MODULE_NAME ${MODULE_NAME_IN})
  
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
    MESSAGE(STATUS "configuring Module ${MODULE_NAME}...") 
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

      IF(MODULE_IS_ENABLED)
	     SET(MODULE_IS_ENABLED 1)
	     _RTTB_CREATE_MODULE_CONF()
	  IF(NOT MODULE_EXPORT_DEFINE)
	    SET(MODULE_EXPORT_DEFINE ${MODULE_NAME}_EXPORT)
	  ENDIF(NOT MODULE_EXPORT_DEFINE)
	  
	  CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/moduleExports.h.in ${RTTB_MODULES_CONF_DIR}/${MODULE_NAME}Exports.h @ONLY)
	
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

	  IF(MODULE_FORCE_STATIC)
	    SET(_STATIC ${RTTB_WIN32_FORCE_STATIC})
	  ENDIF(MODULE_FORCE_STATIC)
    
	  IF(NOT MODULE_HEADERS_ONLY)
        IF(ALL_LIBRARY_DIRS)
		  # LINK_DIRECTORIES applies only to targets which are added after the call to LINK_DIRECTORIES
		  LINK_DIRECTORIES(${ALL_LIBRARY_DIRS})
        ENDIF(ALL_LIBRARY_DIRS)
     
	    SET(coverage_sources ${CPP_FILES} ${H_FILES} ${TXX_FILES})
	    SET_PROPERTY(SOURCE ${coverage_sources} APPEND PROPERTY LABELS ${MODULE_SUBPROJECTS})

	   ADD_LIBRARY(${MODULE_PROVIDES} ${_STATIC} ${coverage_sources} ${CPP_FILES_GENERATED})
	    
       SET_TARGET_PROPERTIES(${MODULE_PROVIDES} PROPERTIES ${RTToolbox_LIBRARY_PROPERTIES} OUTPUT_NAME ${MODULE_PROVIDES}-${RTToolbox_VERSION_MAJOR}.${RTToolbox_VERSION_MINOR} OUTPUT_NAME_DEBUG ${MODULE_PROVIDES}-D-${RTToolbox_VERSION_MAJOR}.${RTToolbox_VERSION_MINOR})		
       INSTALL(TARGETS ${MODULE_PROVIDES} EXPORT RTToolboxTargets LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
  
        IF(ALL_LIBRARIES)
          TARGET_LINK_LIBRARIES(${MODULE_PROVIDES} ${ALL_LIBRARIES})
        ENDIF(ALL_LIBRARIES)  
	  ENDIF()
	  
      ENDIF(MODULE_IS_ENABLED)
    ENDIF(_MISSING_DEP)
  ENDIF(NOT MODULE_IS_EXCLUDED)
    
  IF(NOT MODULE_IS_ENABLED)   
    _RTTB_CREATE_MODULE_CONF()
  ENDIF(NOT MODULE_IS_ENABLED)   
ENDMACRO(RTTB_CREATE_MODULE)
