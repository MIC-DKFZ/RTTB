MESSAGE (STATUS "generating app: DoseAcc4V - simple dose accumulation tool with Virtuos")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseAcc4V DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO PACKAGE_DEPENDS ArgumentParsingLib MatchPoint ITK)

