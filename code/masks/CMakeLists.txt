MESSAGE (STATUS "processing RTToolbox mask")

RTTB_CREATE_MODULE(RTTBMasks DEPENDS RTTBCore PACKAGE_DEPENDS boost)

OPTION(BUILD_Masks_Boost "Determine if the boost mask voxelization will be supported." ON)
IF(BUILD_Masks_Boost)
	ADD_SUBDIRECTORY(boost)
ENDIF(BUILD_Masks_Boost)

OPTION(BUILD_Masks_OTB "Determine if the rttb OTB mask voxalization will be supported." OFF)
IF(BUILD_Masks_OTB)
	ADD_SUBDIRECTORY(legacy)
ENDIF(BUILD_Masks_OTB)




