RTTB_CREATE_MODULE(RTTBDicomIO DEPENDS RTTBCore RTTBAlgorithms PACKAGE_DEPENDS BoostBinaries DCMTK)
IF (NOT WIN32)
	target_compile_features(RTTBDicomIO PRIVATE cxx_auto_type cxx_nullptr cxx_override)
ENDIF()
