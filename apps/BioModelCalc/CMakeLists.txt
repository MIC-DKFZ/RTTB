MESSAGE (STATUS "generating app: BioModelCalc - calculating the radiobiological effect based on dose")

SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(BioModelCalc DEPENDS RTTBCore RTTBModels RTTBUtilsIO PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)

