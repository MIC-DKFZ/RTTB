MESSAGE (STATUS "generating app: DoseTool4V - calculating dose statistics and DVH with Virtuos support")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseTool4V DEPENDS RTTBCore RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO RTTBMasks RTTBMasks RTTBBoostMask RTTBOtherIO RTTBAlgorithms PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)
IF (NOT WIN32)
#	target_compile_features(DoseTool4V PRIVATE cxx_auto_type cxx_nullptr cxx_override)#
#	set(CMAKE_CXX_FLAGS "-fpermissive")
ENDIF()
