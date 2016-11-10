RTTB_CREATE_MODULE(RTTBModels DEPENDS RTTBCore PACKAGE_DEPENDS Boost)

IF (NOT WIN32)
	target_compile_features(RTTBModels PRIVATE cxx_auto_type cxx_nullptr cxx_override)
ENDIF()
