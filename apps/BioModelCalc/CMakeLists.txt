MESSAGE (STATUS "generating app: BioModelCalc - calculating the radiobiological effect based on dose")

SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(BioModelCalc DEPENDS RTTBCore RTTBITKIO RTTBDicomIO RTTBHelaxIO RTTBModels PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)
IF (NOT WIN32)
	target_compile_features(BioModelCalc PRIVATE cxx_auto_type cxx_nullptr cxx_override)
	set(CMAKE_CXX_FLAGS "-fpermissive")
ENDIF()
