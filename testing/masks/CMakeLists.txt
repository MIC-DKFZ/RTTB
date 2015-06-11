
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
  
#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(Masks_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbMasksTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(VOIindexIdentifierTest ${Mask_TESTS} VOIindexIdentifierTest 
"${TEST_DATA_ROOT}/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" )


RTTB_CREATE_TEST_MODULE(rttbMasks DEPENDS RTTBDicomIO RTTBMasks PACKAGE_DEPENDS Boost Litmus DCMTK)
 


