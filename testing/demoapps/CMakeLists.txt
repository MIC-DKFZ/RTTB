
  MESSAGE (STATUS "Process All demoapps Tests...")

  #-----------------------------------------------------------------------------
  # Include sub directories
  #-----------------------------------------------------------------------------
IF(BUILD_DemoApp_DoseTool)
  ADD_SUBDIRECTORY (DoseTool)
ENDIF(BUILD_DemoApp_DoseTool)

IF(BUILD_DemoApp_VoxelizerTool)
  ADD_SUBDIRECTORY (VoxelizerTool)
ENDIF(BUILD_DemoApp_VoxelizerTool)
