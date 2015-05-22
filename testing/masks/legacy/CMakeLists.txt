#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(OTB_MASK_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbOTBMaskTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(OTBMaskAccessorTest ${OTB_MASK_TESTS} OTBMaskAccessorTest 
)

RTTB_CREATE_TEST_MODULE(rttbOTBMask DEPENDS RTTBMasks RTTBOTBMask PACKAGE_DEPENDS Litmus DCMTK)

