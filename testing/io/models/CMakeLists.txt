#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(MODELSIO_TEST ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}ModelsIOTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------


ADD_TEST(ModelsIOTest ${MODELSIO_TEST} ModelsIOTest "${TEST_DATA_ROOT}/BioModel/XML/referenceBioModeltcpleqIOTest.xml" "${TEST_DATA_ROOT}/BioModel/XML/referenceBioModelntcplkIOTest.xml" ) 

RTTB_CREATE_TEST_MODULE(ModelsIO DEPENDS RTTBModelsIO RTTBModels PACKAGE_DEPENDS Boost|filesystem Litmus RTTBData)
