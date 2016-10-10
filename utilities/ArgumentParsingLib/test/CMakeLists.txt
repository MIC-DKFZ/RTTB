if (BUILD_TESTS)
	find_package(Threads REQUIRED) # Necessary under Linux. Has no effect on Windows.

	find_package(GTest)
	if(NOT GTEST_FOUND)
		message(STATUS "GTest will be automatically downloaded and built.")
		include(ExternalProject)
		
		set(GTEST_SOURCE_DIR "${CMAKE_BINARY_DIR}/googletest-src")
		set(GTEST_BUILD_DIR "${CMAKE_BINARY_DIR}/googletest-build")
		set(GTEST_CMAKE_DIR "${CMAKE_BINARY_DIR}/googletest-cmake")
		
		if (CMAKE_VERSION VERSION_LESS 3.2)
			set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
		else()
			set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
		endif()

		include(DownloadProject.cmake)
		download_project(PROJ                googletest
						 GIT_REPOSITORY      https://github.com/google/googletest.git
						 GIT_TAG             master
						 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
		)
		
		# Prevent GoogleTest from overriding our compiler/linker options
		# when building with Visual Studio
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
		SET(BUILD_GTEST ON CACHE BOOL "" FORCE)
		SET(BUILD_GMOCK OFF)

		add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
		
		set(GTEST_INCLUDE_DIR "${GTEST_SOURCE_DIR}/googletest/include")
		link_directories("${GTEST_BUILD_DIR}/googletest") # Has to be called before add_executable
		
		set(GTEST_BOTH_LIBRARIES "${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")
		list(APPEND GTEST_BOTH_LIBRARIES "${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}")
	endif()
	
	set(ABS_MAIN_DIR "${PROJECT_SOURCE_DIR}/${MAIN_DIR}")
	include_directories(${GTEST_INCLUDE_DIR} ${ABS_MAIN_DIR}) # Has to be called before add_executable
	
	set(TEST_PROJECT_NAME "${PROJECT_NAME}Tests")
	set(CPP_FILES
		${ABS_MAIN_DIR}/parseXML.cpp
		${ABS_MAIN_DIR}/CmdLineParserBase.cpp
		${ABS_MAIN_DIR}/XMLGenerator.cpp
		parseXMLTest.cpp
		CmdLineParserBaseTest.cpp
		XMLGeneratorTest.cpp
	)
	
	add_executable(${TEST_PROJECT_NAME} ${CPP_FILES})
	target_link_libraries(${TEST_PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
	
	if(NOT GTEST_FOUND)
		add_dependencies(${TEST_PROJECT_NAME} gtest) # Necessary so the build waits for gtest
	endif()

	# MSVC automatically links to boost
	if(NOT MSVC)
		target_link_libraries(${TEST_PROJECT_NAME} ${Boost_LIBRARIES})
	endif()
	
	# Copy test data files to build dir
	add_custom_command(TARGET ${TEST_PROJECT_NAME} PRE_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
					   ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR})
	
	# Enable C++11 support (doesn't work with MSVC)
	set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY CXX_STANDARD 11)
	set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
endif()