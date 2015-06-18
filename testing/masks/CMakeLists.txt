
  MESSAGE (STATUS "Process All Mask Tests...")

  #-----------------------------------------------------------------------------
  # Include sub directories
  #-----------------------------------------------------------------------------
	
  IF(BUILD_Masks_Boost)
	ADD_SUBDIRECTORY(boost)
  ENDIF()
   
  IF(BUILD_Masks_OTB)
	ADD_SUBDIRECTORY(legacy)
  ENDIF()
 


