MESSAGE(STATUS "processing RTToolbox code")
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(algorithms)

OPTION(BUILD_Interpolation "Determine if the dose interpolation classes will be generated." OFF)
IF(BUILD_All_Modules OR BUILD_Interpolation)
	ADD_SUBDIRECTORY(interpolation)
	SET(BUILD_Interpolation ON CACHE BOOL ON FORCE)
ENDIF()

ADD_SUBDIRECTORY(indices)

OPTION(BUILD_Models "Determine if the model classes will be generated." OFF)
IF(BUILD_All_Modules OR BUILD_Models)
	ADD_SUBDIRECTORY(models)
	SET(BUILD_Models ON CACHE BOOL ON FORCE)
ENDIF()

OPTION(BUILD_Masks "Determine if the mask classes will be generated." ON)
IF(BUILD_All_Modules OR BUILD_Masks)
	ADD_SUBDIRECTORY(masks)
	SET(BUILD_Masks ON CACHE BOOL ON FORCE)
ENDIF()

ADD_SUBDIRECTORY(io)
ADD_SUBDIRECTORY(testhelper)
