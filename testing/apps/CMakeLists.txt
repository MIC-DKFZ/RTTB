
  MESSAGE (STATUS "Process All apps Tests...")

  #-----------------------------------------------------------------------------
  # Include sub directories
  #-----------------------------------------------------------------------------
IF(BUILD_App_DoseTool)
  ADD_SUBDIRECTORY (DoseTool)
ENDIF(BUILD_App_DoseTool)

IF(BUILD_App_VoxelizerTool)
  ADD_SUBDIRECTORY (VoxelizerTool)
ENDIF(BUILD_App_VoxelizerTool)

IF(BUILD_App_DoseMap)
  ADD_SUBDIRECTORY (DoseMap)
ENDIF()

IF(BUILD_App_DoseAcc)
  ADD_SUBDIRECTORY (DoseAcc)
ENDIF()