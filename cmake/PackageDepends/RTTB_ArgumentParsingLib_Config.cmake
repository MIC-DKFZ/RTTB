#-----------------------------------------------------------------------------
# ArgumentParsingLib is built automatically. Just set include dir and link directories
#-----------------------------------------------------------------------------

set(ArgumentParsingLib_INCLUDE_DIR ${ArgumentParsingLib_SOURCE_DIR}/main)
set(ArgumentParsingLib_LIBRARY_DIR ${ArgumentParsingLib_BUILD_DIR}/main)
set(ArgumentParsingLib_Boost_INCLUDE_DIR ${Boost_INCLUDE_DIR})
set(ArgumentParsingLib_Boost_LIBRARY_DIR ${Boost_LIBRARY_DIR})
set(ArgumentParsingLib_Boost_LIBRARIES ${Boost_LIBRARIES})

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${ArgumentParsingLib_INCLUDE_DIR} ${ArgumentParsingLib_Boost_INCLUDE_DIR})
LIST(APPEND ALL_LIBRARIES "${CMAKE_STATIC_LIBRARY_PREFIX}ArgumentParsingLib${CMAKE_STATIC_LIBRARY_SUFFIX}")

LINK_DIRECTORIES(${ArgumentParsingLib_LIBRARY_DIR} ${ArgumentParsingLib_Boost_LIBRARY_DIR}) 