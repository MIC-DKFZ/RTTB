MESSAGE (STATUS "generating app: DoseAcc - simple dose accumulation tool")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseAcc DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBUtilsIO PACKAGE_DEPENDS ArgumentParsingLib MatchPoint ITK BoostBinaries)

