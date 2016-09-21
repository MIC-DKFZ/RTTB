MESSAGE (STATUS "generating demo app: DoseAcc - simple dose accumulation tool example")

RTTB_CREATE_APPLICATION(DoseAcc DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBITKIO RTTBDicomIO RTTBHelaxIO PACKAGE_DEPENDS ArgumentParsingLib MatchPoint ITK)