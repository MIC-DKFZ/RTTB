MESSAGE (STATUS "generating app: BioModelCalc4V - calculating the radiobiological effect based on dose with Virtuos support")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(BioModelCalc4V DEPENDS RTTBCore RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO RTTBModels PACKAGE_DEPENDS ArgumentParsingLib BoostBinaries)

