
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
  
#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(Masks_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbMasksTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(VOIindexIdentifierTest ${Masks_TESTS} VOIindexIdentifierTest 
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" )


RTTB_CREATE_TEST_MODULE(rttbMasks DEPENDS RTTBDicomIO RTTBMasks PACKAGE_DEPENDS Boost Litmus DCMTK)
 


