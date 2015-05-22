
  MESSAGE (STATUS "Process All Mask Tests...")

  #-----------------------------------------------------------------------------
  # Include sub directories
  #-----------------------------------------------------------------------------

  IF(BUILD_MASK_BOOST)
	ADD_SUBDIRECTORY(boost)
  ENDIF(BUILD_MASK_BOOST)
   
  IF(BUILD_OTBMASK)
	ADD_SUBDIRECTORY(legacy)
  ENDIF(BUILD_OTBMASK)
 


