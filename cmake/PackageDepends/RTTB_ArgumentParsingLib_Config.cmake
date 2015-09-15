#-----------------------------------------------------------------------------
# Find ArgumentParsingLib
#-----------------------------------------------------------------------------
FIND_PACKAGE(ArgumentParsingLib REQUIRED)
if (ArgumentParsingLib_DIR)
	include(${ArgumentParsingLib_DIR}/ArgumentParsingLibConfig.cmake)
	set(ArgumentParsingLib_INCLUDE_DIR ${ArgumentParsingLib_SOURCE_DIR}/main)
	set(ArgumentParsingLib_LIBRARY_DIR ${ArgumentParsingLib_BINARY_DIR}/main)
	set(ArgumentParsingLib_Boost_INCLUDE_DIR ${ArgumentParsingLib_Boost_INCLUDE_DIR})
	set(ArgumentParsingLib_Boost_LIBRARY_DIR ${ArgumentParsingLib_Boost_LIBRARY_DIR})
	set(ArgumentParsingLib_Boost_LIBRARIES ${ArgumentParsingLib_Boost_LIBRARIES})
else()
	message(FATAL_ERROR "Missing build directory of ArgumentParsingLib. Please specify it manually.")
endif()
LIST(APPEND ALL_INCLUDE_DIRECTORIES ${ArgumentParsingLib_INCLUDE_DIR} ${ArgumentParsingLib_Boost_INCLUDE_DIR})
LIST(APPEND ALL_LIBRARIES "${CMAKE_STATIC_LIBRARY_PREFIX}ArgumentParsingLib${CMAKE_STATIC_LIBRARY_SUFFIX}")

LINK_DIRECTORIES(${ArgumentParsingLib_LIBRARY_DIR} ${ArgumentParsingLib_Boost_LIBRARY_DIR}) 