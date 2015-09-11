MESSAGE (STATUS "generating demo app: VoxelizerTool - writing files of voxelized structures")

RTTB_CREATE_APPLICATION(VoxelizerTool DEPENDS RTTBITKIO RTTBOTBMask RTTBBoostMask RTTBDicomIO RTTBMasks RTTBCore PACKAGE_DEPENDS MatchPoint ITK)