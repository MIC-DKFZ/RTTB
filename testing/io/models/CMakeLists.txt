#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(MODELSIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbModelsIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------


ADD_TEST(ModelsIOTest ${MODELSIO_TEST} ModelsIOTest "${TEST_DATA_ROOT}/BioModel/XML/referenceBioModeltcpleqIOTest.xml" "${TEST_DATA_ROOT}/BioModel/XML/referenceBioModelntcplkIOTest.xml" ) 

RTTB_CREATE_TEST_MODULE(rttbModelsIO DEPENDS RTTBModelsIO RTTBModels PACKAGE_DEPENDS Boost Litmus)