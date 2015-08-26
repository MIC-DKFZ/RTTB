MESSAGE(STATUS "processing RTToolbox demo apps")

OPTION(BUILD_DemoApp_DoseAcc "Determine if the demo application DoseAcc will be generated." OFF)
IF(BUILD_DemoApp_DoseAcc)
ADD_SUBDIRECTORY(DoseAcc)
ENDIF(BUILD_DemoApp_DoseAcc)

OPTION(BUILD_DemoApp_VoxelizerTool "Determine if the demo application VoxelizerTool will be generated." OFF)
IF(BUILD_DemoApp_VoxelizerTool)
ADD_SUBDIRECTORY(VoxelizerTool)
ENDIF(BUILD_DemoApp_VoxelizerTool)