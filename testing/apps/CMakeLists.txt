MESSAGE (STATUS "Process All apps Tests...")

#-----------------------------------------------------------------------------
# Include sub directories
#-----------------------------------------------------------------------------
IF(BUILD_App_DoseTool)
  ADD_SUBDIRECTORY (DoseTool)
  IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
	ADD_SUBDIRECTORY (DoseTool4V)
  ENDIF()
ENDIF()

IF(BUILD_App_VoxelizerTool)
  ADD_SUBDIRECTORY (VoxelizerTool)
ENDIF()

IF(BUILD_App_DoseMap AND BUILD_IO_Virtuos)
  ADD_SUBDIRECTORY (DoseMap)
ENDIF()

IF(BUILD_App_BioModelCalc)
  ADD_SUBDIRECTORY (BioModelCalc)
ENDIF()

IF(BUILD_App_DoseAcc)
  ADD_SUBDIRECTORY (DoseAcc)
ENDIF()