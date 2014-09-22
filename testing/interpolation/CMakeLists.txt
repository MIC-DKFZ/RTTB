#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(INTERPOLATION_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbInterpolationTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(SimpleMappableDoseAccessorTest ${INTERPOLATION_TESTS} SimpleMappableDoseAccessorTest "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncreaseX.dcm")
ADD_TEST(RosuMappableDoseAccessorTest ${INTERPOLATION_TESTS} RosuMappableDoseAccessorTest "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncreaseX.dcm")
ADD_TEST(InterpolationTest ${INTERPOLATION_TESTS} InterpolationTest "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncreaseX.dcm")

RTTB_CREATE_TEST_MODULE(rttbInterpolation DEPENDS RTTBInterpolation RTTBMatchPointBinding RTTBDicomIO RTTBITKIO RTTBHelaxIO PACKAGE_DEPENDS Boost Litmus MatchPoint DCMTK)
