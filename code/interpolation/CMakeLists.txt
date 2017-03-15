RTTB_CREATE_MODULE(RTTBInterpolation DEPENDS RTTBCore PACKAGE_DEPENDS Boost)

ADD_SUBDIRECTORY(ITKTransformation)

OPTION(BUILD_InterpolationMatchPointTransformation
	   "Determine if the MatchPoint transformation for dose interpolation classes will be generated." OFF)
IF(BUILD_All_Modules OR BUILD_InterpolationMatchPointTransformation)
	ADD_SUBDIRECTORY(MatchPointTransformation)
	SET(BUILD_InterpolationMatchPointTransformation ON CACHE BOOL ON FORCE)
ENDIF()

