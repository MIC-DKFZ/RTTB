MESSAGE (STATUS "processing RTToolbox mask")

OPTION(BUILD_MASK_BOOST "Determine if the boost mask voxelization will be supported." ON)
IF(BUILD_MASK_BOOST)
	ADD_SUBDIRECTORY(boost)
ENDIF(BUILD_MASK_BOOST)

OPTION(BUILD_OTBMASK "Determine if the rttb OTB mask voxalization will be supported." ON)
IF(BUILD_OTBMASK)
	ADD_SUBDIRECTORY(legacy)
ENDIF(BUILD_OTBMASK)

RTTB_CREATE_MODULE(RTTBMasks DEPENDS RTTBCore PACKAGE_DEPENDS boost)


