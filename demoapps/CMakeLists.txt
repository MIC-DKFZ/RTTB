MESSAGE(STATUS "processing RTToolbox demo apps")

OPTION(BUILD_DemoApp_DoseAcc "Determine if the demo application DoseAcc will be generated." OFF)
IF(BUILD_DemoApp_DoseAcc)
ADD_SUBDIRECTORY(DoseAcc)
ENDIF(BUILD_DemoApp_DoseAcc)
