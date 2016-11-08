SET(RTTB_Boost_ADDITIONAL_COMPONENT thread)
RTTB_CREATE_MODULE(RTTBBoostMask DEPENDS RTTBCore RTTBMasks PACKAGE_DEPENDS BoostBinaries)

IF (NOT WIN32)
	target_compile_features(RTTBBoostMask PRIVATE cxx_auto_type cxx_nullptr cxx_override)
ENDIF()
