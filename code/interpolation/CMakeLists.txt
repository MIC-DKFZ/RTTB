OPTION(BUILD_InterpolationMatchPointTransformation
	   "Determine if the MatchPoint transformation for dose interpolation classes will be generated." OFF)
IF(BUILD_All_Modules OR BUILD_InterpolationMatchPointTransformation)
	ADD_SUBDIRECTORY(MatchPointTransformation)
	SET(BUILD_InterpolationMatchPointTransformation ON CACHE BOOL ON FORCE)
ENDIF()
ADD_SUBDIRECTORY(ITKTransformation)

RTTB_CREATE_MODULE(RTTBInterpolation DEPENDS RTTBCore PACKAGE_DEPENDS Boost)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
