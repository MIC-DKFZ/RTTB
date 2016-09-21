MESSAGE (STATUS "generating app: DoseAcc4V - simple dose accumulation tool with Virtuos")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseAcc4V DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO PACKAGE_DEPENDS ArgumentParsingLib MatchPoint ITK)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive")
ENDIF()
