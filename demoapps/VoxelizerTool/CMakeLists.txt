MESSAGE (STATUS "generating demo app: VoxelizerTool - simple dose accumulation tool example")

RTTB_CREATE_APPLICATION(RTTBVoxelizerTool DEPENDS RTTBITKIO RTTBOTBMask RTTBBoostMask RTTBDicomIO RTTBMasks RTTBAlgorithms RTTBInterpolation RTTBCore PACKAGE_DEPENDS MatchPoint ITK)