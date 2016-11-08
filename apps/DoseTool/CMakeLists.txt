MESSAGE (STATUS "generating demo app: DoseTool - calculating dose statistics of a structure")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseTool DEPENDS RTTBCore RTTBITKIO RTTBDicomIO RTTBHelaxIO RTTBMasks RTTBMasks RTTBBoostMask RTTBOtherIO RTTBAlgorithms PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)

IF (NOT WIN32)
	target_compile_features(DoseTool PRIVATE cxx_auto_type cxx_nullptr cxx_override)
	set(CMAKE_CXX_FLAGS "-fpermissive")
ENDIF()
