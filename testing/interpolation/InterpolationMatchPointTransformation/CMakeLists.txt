#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(INTERPOLATION_MP_TRANSFORMATION_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbInterpolationMPTransformationTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(SimpleMappableDoseAccessorWithMatchPointTest ${INTERPOLATION_MP_TRANSFORMATION_TESTS} SimpleMappableDoseAccessorWithMatchPointTest "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncreaseX.dcm")

RTTB_CREATE_TEST_MODULE(rttbInterpolationMPTransformation DEPENDS RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBDicomIO PACKAGE_DEPENDS Litmus MatchPoint)
