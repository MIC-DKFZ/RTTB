MESSAGE (STATUS "generating demo app: BioModelCalc - calculating the radiobiological effect based on dose")

RTTB_CREATE_APPLICATION(BioModelCalc DEPENDS RTTBCore RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO RTTBModels PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive")
ENDIF()
