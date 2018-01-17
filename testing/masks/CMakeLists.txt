
MESSAGE (STATUS "Process All Mask Tests...")

#-----------------------------------------------------------------------------
# Include sub directories
#-----------------------------------------------------------------------------

ADD_SUBDIRECTORY(boost)
   
IF(BUILD_Masks_Legacy)
	ADD_SUBDIRECTORY(legacy)
ENDIF()