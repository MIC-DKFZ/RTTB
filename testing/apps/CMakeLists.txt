
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

IF(BUILD_App_DoseMap AND BUILD_IO_Virtuos)
  ADD_SUBDIRECTORY (DoseMap)
ENDIF()
IF(BUILD_App_BioModelCalc)
  ADD_SUBDIRECTORY (BioModelCalc)
ENDIF()

IF(BUILD_App_DoseAcc)
  ADD_SUBDIRECTORY (DoseAcc)
ENDIF()