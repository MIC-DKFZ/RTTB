MESSAGE (STATUS "generating app: DoseMap - simple dose mapping tool")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(DoseMap DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBUtilsIO PACKAGE_DEPENDS MatchPoint ITK ArgumentParsingLib BoostBinaries)
