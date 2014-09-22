OPTION(BUILD_MatchPointBinding
	   "Determine if the MatchPoint binding for dose interpolation classes will be generated." OFF)
IF(BUILD_MatchPointBinding)
ADD_SUBDIRECTORY(MatchPointBinding)
ENDIF(BUILD_MatchPointBinding)

RTTB_CREATE_MODULE(RTTBInterpolation DEPENDS RTTBCore PACKAGE_DEPENDS Boost)