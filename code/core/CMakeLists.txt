RTTB_CREATE_MODULE(RTTBCore PACKAGE_DEPENDS BoostBinaries)

IF (NOT WIN32)
	target_compile_features(RTTBCore PRIVATE cxx_auto_type cxx_nullptr cxx_override)
ENDIF()
