MESSAGE(STATUS "processing RTToolbox code")
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(algorithms)
ADD_SUBDIRECTORY(indices)


OPTION(BUILD_Models "Determine if the model classes will be generated." OFF)
IF(BUILD_Models)
ADD_SUBDIRECTORY(models)
ENDIF(BUILD_Models)

OPTION(BUILD_Masks "Determine if the mask classes will be generated." ON)
IF(BUILD_Masks)
ADD_SUBDIRECTORY(masks)
ENDIF(BUILD_Masks)

OPTION(BUILD_Interpolation "Determine if the dose interpolation classes will be generated." OFF)
IF(BUILD_Interpolation)
ADD_SUBDIRECTORY(interpolation)
ENDIF(BUILD_Interpolation)

ADD_SUBDIRECTORY(io)
