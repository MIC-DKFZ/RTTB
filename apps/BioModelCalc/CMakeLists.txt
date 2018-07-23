MESSAGE (STATUS "generating app: BioModelCalc - calculating the radiobiological effect based on dose")

SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(RTTBBioModelCalc DEPENDS RTTBCore RTTBModels RTTBUtilsIO PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)
SET_TARGET_PROPERTIES(RTTBBioModelCalc PROPERTIES OUTPUT_NAME "BioModelCalc")
