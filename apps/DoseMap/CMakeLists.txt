MESSAGE (STATUS "generating app: DoseMap - simple dose mapping tool")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseMap DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO PACKAGE_DEPENDS MatchPoint ITK ArgumentParsingLib BoostBinaries)

IF (NOT WIN32)
	target_compile_features(DoseMap PRIVATE cxx_auto_type cxx_nullptr cxx_override)
	set(CMAKE_CXX_FLAGS "-fpermissive")
ENDIF()
