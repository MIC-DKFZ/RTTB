RTTB_CREATE_MODULE(RTTBAlgorithms DEPENDS RTTBCore PACKAGE_DEPENDS Boost)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
