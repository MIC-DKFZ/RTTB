MESSAGE (STATUS "generating app: DoseAcc - simple dose accumulation tool")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(RTTBDoseAcc DEPENDS RTTBCore RTTBAlgorithms RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBUtilsIO PACKAGE_DEPENDS ArgumentParsingLib MatchPoint ITK Boost)
SET_TARGET_PROPERTIES(RTTBDoseAcc PROPERTIES OUTPUT_NAME "DoseAcc")
