
  MESSAGE (STATUS "Process All IO Tests...")

  #-----------------------------------------------------------------------------
  # Include sub directories
  #-----------------------------------------------------------------------------

  ADD_SUBDIRECTORY (other)

  IF(BUILD_IO_Dicom)
	ADD_SUBDIRECTORY(dicom)
	IF(BUILD_IO_HELAX)
		ADD_SUBDIRECTORY(helax)
	ENDIF(BUILD_IO_HELAX)
  ENDIF(BUILD_IO_Dicom)

  IF(BUILD_IO_ITK)
	ADD_SUBDIRECTORY(itk)
  ENDIF(BUILD_IO_ITK)
  
  IF(BUILD_Models)
	ADD_SUBDIRECTORY(models)
  ENDIF()
   
IF(BUILD_IO_Virtuos)
ADD_SUBDIRECTORY(virtuos)
ENDIF(BUILD_IO_Virtuos)