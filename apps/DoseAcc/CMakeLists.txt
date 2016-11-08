MESSAGE (STATUS "generating demo app: DoseAcc - simple dose accumulation tool example")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseAcc DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBITKIO RTTBDicomIO RTTBHelaxIO PACKAGE_DEPENDS ArgumentParsingLib MatchPoint ITK BoostBinaries)

IF (NOT WIN32)
	target_compile_features(DoseAcc PRIVATE cxx_auto_type cxx_nullptr cxx_override)
	set(CMAKE_CXX_FLAGS "-fpermissive")
ENDIF()
