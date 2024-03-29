#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(MODELS_TESTS ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}ModelTests)
SET(MODELS_HEADER_TEST ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}ModelsHeaderTest)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(BioModelTest ${MODELS_TESTS} BioModelTest)
ADD_TEST(BioModelScatterPlotTest ${MODELS_TESTS} BioModelScatterPlotTest)
ADD_TEST(DvhBasedModelsTest ${MODELS_TESTS} DvhBasedModelsTest)
ADD_TEST(LQModelAccessorTest ${MODELS_TESTS} LQModelAccessorTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncreaseX.dcm")

RTTB_CREATE_TEST_MODULE(Model DEPENDS RTTBModels RTTBDicomIO RTTBTestHelper PACKAGE_DEPENDS Boost Litmus DCMTK RTTBData)

