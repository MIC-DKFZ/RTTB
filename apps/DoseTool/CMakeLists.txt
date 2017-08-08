MESSAGE (STATUS "generating app: DoseTool - calculating dose statistics and DVH")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseTool DEPENDS RTTBCore RTTBMasks RTTBMasks RTTBBoostMask RTTBOtherIO RTTBAlgorithms RTTBUtilsIO PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)

