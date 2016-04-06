MESSAGE (STATUS "generating app: DoseAcc - simple dose accumulation tool example")

RTTB_CREATE_APPLICATION(DoseAcc DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBITKIO RTTBVirtuosIO RTTBDicomIO RTTBHelaxIO PACKAGE_DEPENDS MatchPoint ITK)