#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(INTERPOLATION_MP_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbInterpolationMPTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(SimpleMappableDoseAccessorWithMatchPointTest ${INTERPOLATION_MP_TESTS} SimpleMappableDoseAccessorWithMatchPointTest "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncreaseX.dcm")

RTTB_CREATE_TEST_MODULE(rttbInterpolationMP DEPENDS RTTBInterpolation RTTBMatchPointBinding RTTBDicomIO PACKAGE_DEPENDS Litmus MatchPoint)
