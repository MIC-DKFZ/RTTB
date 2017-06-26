#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(INTERPOLATION_ITK_TRANSFORMATION_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbInterpolationITKTransformationTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(SimpleMappableDoseAccessorWithITKTest ${INTERPOLATION_ITK_TRANSFORMATION_TESTS} SimpleMappableDoseAccessorWithITKTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncreaseX.dcm")

RTTB_CREATE_TEST_MODULE(rttbInterpolationITKTransformation DEPENDS RTTBInterpolation RTTBInterpolationITKTransformation RTTBDicomIO PACKAGE_DEPENDS Litmus RTTBData ITK)
