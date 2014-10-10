OPTION(BUILD_MatchPointBinding
	   "Determine if the MatchPoint binding for dose interpolation classes will be generated." OFF)
OPTION(BUILD_ITKBinding
	   "Determine if the ITK binding for dose interpolation classes will be generated." OFF)
IF(BUILD_MatchPointBinding)
	ADD_SUBDIRECTORY(MatchPointBinding)
ENDIF(BUILD_MatchPointBinding)
IF(BUILD_ITKBinding)
	ADD_SUBDIRECTORY(ITKBinding)
ENDIF(BUILD_ITKBinding)

RTTB_CREATE_MODULE(RTTBInterpolation DEPENDS RTTBCore PACKAGE_DEPENDS Boost)