OPTION(BUILD_MatchPointBinding
	   "Determine if the MatchPoint binding for dose interpolation classes will be generated." OFF)
OPTION(BUILD_ITKBinding
	   "Determine if the ITK binding for dose interpolation classes will be generated." OFF)
IF(BUILD_MatchPointBinding)
	ADD_SUBDIRECTORY(MatchPointBinding)
ENDIF(BUILD_MatchPointBinding)
IF(BUILD_ITKBinding)
	ADD_SUBDIRECTORY(ITKBinding)
ENDIF(BUILD_ITKBinding)

RTTB_CREATE_MODULE(RTTBInterpolation DEPENDS RTTBCore PACKAGE_DEPENDS Boost)
IF (NOT WIN32)
		#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
