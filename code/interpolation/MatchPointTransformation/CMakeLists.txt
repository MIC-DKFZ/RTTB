RTTB_CREATE_MODULE(RTTBInterpolationMatchPointTransformation DEPENDS RTTBCore RTTBInterpolation PACKAGE_DEPENDS Boost MatchPoint ITK)

IF (NOT WIN32)
	target_compile_features(RTTBInterpolationMatchPointTransformation PRIVATE cxx_auto_type cxx_nullptr cxx_override)
	set(CMAKE_CXX_FLAGS "-fpermissive")
ENDIF()
