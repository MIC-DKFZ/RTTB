#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(LEGACY_MASK_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbLegacyMaskTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(OTBMaskAccessorTest ${LEGACY_MASK_TESTS} OTBMaskAccessorTest 
)

ADD_TEST(BoostMaskLegacyTest ${LEGACY_MASK_TESTS} BoostMaskLegacyTest 
)

RTTB_CREATE_TEST_MODULE(rttbLegacyMask DEPENDS RTTBMasks RTTBLegacyMask PACKAGE_DEPENDS Litmus DCMTK Boost)

