MESSAGE (STATUS "generating demo app: BioModelCalc - calculating the radiobiological effect based on dose")

RTTB_CREATE_APPLICATION(BioModelCalc DEPENDS RTTBCore RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO RTTBModels PACKAGE_DEPENDS ArgumentParsingLib)