MESSAGE (STATUS "generating app: VoxelizerTool - voxelize RTSRUCT files")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(VoxelizerTool DEPENDS RTTBITKIO RTTBBoostMask RTTBDicomIO RTTBMasks RTTBCore PACKAGE_DEPENDS ArgumentParsingLib ITK BoostBinaries)

