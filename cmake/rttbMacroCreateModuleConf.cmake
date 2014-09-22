###################################################################
#
# RTTB_CREATE_MODULE_CONF 
# 
# This can be called in a similar way like RTTB_CREATE_MODULE
# but it just creates the module configuration files without
# actually building it. It is used for integration of legacy libraries
# into the RTTB module build system
#
##################################################################
MACRO(RTTB_CREATE_MODULE_CONF MODULE_NAME_IN)
  MACRO_PARSE_ARGUMENTS(MODULE "INCLUDE_DIRS;DEPENDS;PROVIDES" "QT_MODULE;HEADERS_ONLY" ${ARGN})
  SET(MODULE_NAME ${MODULE_NAME_IN})
  SET(MODULE_IS_ENABLED 1)
  _RTTB_CREATE_MODULE_CONF()
ENDMACRO(RTTB_CREATE_MODULE_CONF)

MACRO(_RTTB_CREATE_MODULE_CONF)
    IF(NOT MODULE_PROVIDES AND NOT MODULE_HEADERS_ONLY)
      SET(MODULE_PROVIDES ${MODULE_NAME})
    ENDIF(NOT MODULE_PROVIDES AND NOT MODULE_HEADERS_ONLY)
    SET(MODULE_INCLUDE_DIRS_ABSOLUTE "")
    FOREACH(dir ${MODULE_INCLUDE_DIRS})
      GET_FILENAME_COMPONENT(abs_dir ${dir} ABSOLUTE)
      SET(MODULE_INCLUDE_DIRS_ABSOLUTE ${MODULE_INCLUDE_DIRS_ABSOLUTE} ${abs_dir})
    ENDFOREACH(dir)

    SET(MODULE_INCLUDE_DIRS ${MODULE_INCLUDE_DIRS_ABSOLUTE} ${CMAKE_CURRENT_SOURCE_DIR})
    
    # Qt generates headers in the binary tree
    IF(MODULE_QT_MODULE)
      SET(MODULE_INCLUDE_DIRS ${MODULE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
    ENDIF(MODULE_QT_MODULE)
    
    CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/cmake/moduleConf.cmake.in ${RTTB_MODULES_CONF_DIR}/${MODULE_NAME}Config.cmake @ONLY)
ENDMACRO(_RTTB_CREATE_MODULE_CONF)
