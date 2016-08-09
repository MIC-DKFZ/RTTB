MESSAGE (STATUS "generating demo app: DoseMap - simple dose mapping tool example")

RTTB_CREATE_APPLICATION(DoseMap DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO PACKAGE_DEPENDS MatchPoint ITK ArgumentParsingLib BoostBinaries)