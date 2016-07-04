MESSAGE (STATUS "generating demo app: BioModelCalc4V - calculating the radiobiological effect based on dose")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(BioModelCalc4V DEPENDS RTTBCore RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO RTTBModels PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive")
ENDIF()
