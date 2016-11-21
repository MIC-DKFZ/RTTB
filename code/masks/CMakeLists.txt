MESSAGE (STATUS "processing RTToolbox mask")

RTTB_CREATE_MODULE(RTTBMasks DEPENDS RTTBCore PACKAGE_DEPENDS BoostBinaries)

ADD_SUBDIRECTORY(boost)

OPTION(BUILD_Masks_Legacy "Determine if the rttb legacy mask voxalization will be supported." OFF)
IF(BUILD_Masks_Legacy)
	ADD_SUBDIRECTORY(legacy)
ENDIF(BUILD_Masks_Legacy)




