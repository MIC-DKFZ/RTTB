#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(MODELS_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbModelTests)
SET(MODELS_HEADER_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbModelsHeaderTest)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(BioModelTest ${MODELS_TESTS} BioModelTest)
ADD_TEST(BioModelScatterPlotTest ${MODELS_TESTS} BioModelScatterPlotTest)
ADD_TEST(DvhBasedModelsTest ${MODELS_TESTS} DvhBasedModelsTest)

RTTB_CREATE_TEST_MODULE(rttbModel DEPENDS RTTBModels RTTBOtherIO PACKAGE_DEPENDS Boost Litmus DCMTK)

