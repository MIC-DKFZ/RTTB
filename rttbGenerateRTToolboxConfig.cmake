# Generate the RTToolboxConfig.cmake file in the build tree.  Also configure
# one for installation.  The file tells external projects how to use
# RTToolbox.

#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# Library directory.
SET(RTToolbox_LIBRARY_DIRS_CONFIG ${RTToolbox_LIBRARY_PATH})

# Determine the include directories needed.
SET(RTToolbox_INCLUDE_DIRS_CONFIG "")
FILE(GLOB RTTB_confFiles "${RTTB_MODULES_CONF_DIR}/RTTB*Config.cmake")
FOREACH(RTTB_confFile ${RTTB_confFiles})
    INCLUDE(${RTTB_confFile})
	IF (${${MODULE_CURRENT_NAME}_IS_ENABLED})
		SET(RTToolbox_INCLUDE_DIRS_CONFIG ${RTToolbox_INCLUDE_DIRS_CONFIG} ${${MODULE_CURRENT_NAME}_INCLUDE_DIRS})
	ENDIF (${${MODULE_CURRENT_NAME}_IS_ENABLED})
ENDFOREACH(RTTB_confFile)

SET(RTToolbox_INCLUDE_DIRS_CONFIG ${RTToolbox_BINARY_DIR} ${RTTB_MODULES_CONF_DIR}
  ${RTToolbox_INCLUDE_DIRS_CONFIG}
)

#-----------------------------------------------------------------------------
# Configure RTToolboxConfig.cmake for the build tree.
CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/RTToolboxConfig.cmake.in
               ${RTToolbox_BINARY_DIR}/RTToolboxConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# Include directories.
SET(RTToolbox_INCLUDE_DIRS_CONFIG
  ${RTToolbox_INCLUDE_DIRS_INSTALL_TREE}
  ${RTToolbox_INCLUDE_DIRS_SYSTEM}
)

# Link directories.
SET(RTToolbox_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}${RTToolbox_INSTALL_LIB_DIR})

#-----------------------------------------------------------------------------
# Configure RTToolboxConfig.cmake for the install tree.
CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/RTToolboxConfig.cmake.in
               ${RTToolbox_BINARY_DIR}/RTToolboxConfig.cmake.install @ONLY IMMEDIATE)
