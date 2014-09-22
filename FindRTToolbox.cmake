#
# Find an RTToolbox installation or build tree.
#
# When RTToolbox is found, the RTToolboxConfig.cmake file is sourced to setup the
# location and configuration of RTToolbox.  Please read this file, or
# RTToolboxConfig.cmake.in from the RTToolbox source tree for the full list of
# definitions.  Of particular interest is
#
# RTToolbox_USE_FILE   - A CMake source file that can be included
#                         to set the include directories, library
#                         directories, and preprocessor macros.
#
# In addition to the variables read from RTToolboxConfig.cmake, this find
# module also defines
#
# RTToolbox_DIR        - The directory containing RTToolboxConfig.cmake.  This is either
#                         the root of the build tree, or the lib/InsightToolkit
#                         directory.  This is the only cache entry.
#
# RTToolbox_FOUND      - Whether RTToolbox was found.  If this is true, RTToolbox_DIR is okay.
#
# USE_RTToolbox_FILE   - The full path to the UseRTToolbox.cmake file.  This is provided
#                         for backward compatability.  Use RTToolbox_USE_FILE instead.
#

SET(RTToolbox_DIR_STRING "directory containing RTToolboxConfig.cmake.  This is either the root of the build tree, or PREFIX/lib/RTToolbox for an installation.")

# Search only if the location is not already known.
IF(NOT RTToolbox_DIR)
  # Get the system search path as a list.
  IF(UNIX)
    STRING(REGEX MATCHALL "[^:]+" RTToolbox_DIR_SEARCH1 "$ENV{PATH}")
  ELSE(UNIX)
    STRING(REGEX REPLACE "\\\\" "/" RTToolbox_DIR_SEARCH1 "$ENV{PATH}")
  ENDIF(UNIX)
  STRING(REGEX REPLACE "/;" ";" RTToolbox_DIR_SEARCH2 ${RTToolbox_DIR_SEARCH1})

  # Construct a set of paths relative to the system search path.
  SET(RTToolbox_DIR_SEARCH "")
  FOREACH(dir ${RTToolbox_DIR_SEARCH2})
    SET(RTToolbox_DIR_SEARCH ${ITK_DIR_SEARCH} "${dir}/../lib/RTToolbox")
  ENDFOREACH(dir)

  #
  # Look for an installation or build tree.
  #
  FIND_PATH(RTToolbox_DIR RTToolboxConfig.cmake
    # Look for an environment variable RTToolbox_DIR.
    $ENV{RTToolbox_DIR}

    # Look in places relative to the system executable search path.
    ${RTToolbox_DIR_SEARCH}

    # Look in standard UNIX install locations.
    /usr/local/lib/RTToolbox
    /usr/lib/RTToolbox

    # Read from the CMakeSetup registry entries.  It is likely that
    # RTToolbox will have been recently built.
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild1]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild2]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild3]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild4]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild5]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild6]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild7]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild8]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild9]
    [HKEY_CURRENT_USER\\Software\\Kitware\\CMakeSetup\\Settings\\StartPath;WhereBuild10]

    # Help the user find it if we cannot.
    DOC "The ${RTToolbox_DIR_STRING}"
  )
ENDIF(NOT RTToolbox_DIR)

# If RTToolbox was found, load the configuration file to get the rest of the
# settings.
IF(RTToolbox_DIR)
  SET(RTToolbox_FOUND 1)
  INCLUDE(${RTToolbox_DIR}/RTToolboxConfig.cmake)

  # Set USE_RTToolbox_FILE for backward-compatability.
  SET(USE_RTToolbox_FILE ${RTToolbox_USE_FILE})
ELSE(RTToolbox_DIR)
  SET(RTToolbox_FOUND 0)
  IF(RTToolbox_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Please set RTToolbox_DIR to the ${RTToolbox_DIR_STRING}")
  ENDIF(RTToolbox_FIND_REQUIRED)
ENDIF(RTToolbox_DIR)
