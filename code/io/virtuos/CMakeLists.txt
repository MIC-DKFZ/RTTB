RTTB_CREATE_MODULE(RTTBVirtuosIO DEPENDS RTTBCore PACKAGE_DEPENDS BoostBinaries VirtuosIO)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
