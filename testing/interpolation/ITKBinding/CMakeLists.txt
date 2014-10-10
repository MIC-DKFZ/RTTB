#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(INTERPOLATION_ITK_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbInterpolationITKTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(SimpleMappableDoseAccessorWithITKTest ${INTERPOLATION_ITK_TESTS} SimpleMappableDoseAccessorWithITKTest "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncreaseX.dcm")

RTTB_CREATE_TEST_MODULE(rttbInterpolationITK DEPENDS RTTBInterpolation RTTBITKBinding RTTBDicomIO PACKAGE_DEPENDS Litmus ITK)
