cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(ArgumentParsingLib)

# Disallow compilers that don't support the necessary C++11 features (since the property CXX_STANDARD doesn't work with MSVC)
if(MSVC AND MSVC_VERSION LESS 1800)
	message(FATAL_ERROR "The C++11 features in this project require Visual Studio 2013 or higher")
endif()

set(BUILD_TESTS false CACHE BOOL "Build tests")
set(MAIN_DIR main)
set(TEST_DIR test)

# Sources AND tests both need boost, so include it here
IF (WIN32)
	SET(Boost_USE_STATIC_LIBS ON)
ELSE()
	SET(Boost_USE_STATIC_LIBS OFF)
ENDIF()
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(${MAIN_DIR})
add_subdirectory(${TEST_DIR})

# Create a file by which this project can be found
CONFIGURE_FILE(${ArgumentParsingLib_SOURCE_DIR}/ArgumentParsingLibConfig.cmake.in
               ${ArgumentParsingLib_BINARY_DIR}/ArgumentParsingLibConfig.cmake @ONLY IMMEDIATE)