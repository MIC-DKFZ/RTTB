MESSAGE (STATUS "generating demo app: DoseTool - calculating dose statistics of a structure")

RTTB_CREATE_APPLICATION(DoseTool DEPENDS RTTBCore RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO RTTBMasks RTTBMasks RTTBBoostMask RTTBOtherIO RTTBAlgorithms PACKAGE_DEPENDS ArgumentParsingLib)