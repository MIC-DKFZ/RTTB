MESSAGE (STATUS "generating app: DoseTool - calculating dose statistics and DVH")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(RTTBDoseTool DEPENDS RTTBCore RTTBMask RTTBOtherIO RTTBAlgorithms RTTBUtilsIO PACKAGE_DEPENDS ArgumentParsingLib Boost)
SET_TARGET_PROPERTIES(RTTBDoseTool PROPERTIES OUTPUT_NAME "DoseTool")

