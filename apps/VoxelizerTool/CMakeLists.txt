MESSAGE (STATUS "generating app: VoxelizerTool - voxelize DICOM RTSTRUCT files")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(RTTBVoxelizerTool DEPENDS RTTBMask RTTBCore RTTBUtilsIO PACKAGE_DEPENDS ArgumentParsingLib ITK Boost)
SET_TARGET_PROPERTIES(RTTBVoxelizerTool PROPERTIES OUTPUT_NAME "VoxelizerTool")
