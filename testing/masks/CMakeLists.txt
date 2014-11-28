#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(MASKS_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbMasksTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(OTBMaskAccessorTest ${MASKS_TESTS} OTBMaskAccessorTest 
)

RTTB_CREATE_TEST_MODULE(rttbMasks DEPENDS RTTBMasks PACKAGE_DEPENDS BoostBinaries Litmus DCMTK)

