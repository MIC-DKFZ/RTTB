#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(INTERPOLATION_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbInterpolationTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(SimpleMappableDoseAccessorTest ${INTERPOLATION_TESTS} SimpleMappableDoseAccessorTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncreaseX.dcm")
ADD_TEST(RosuMappableDoseAccessorTest ${INTERPOLATION_TESTS} RosuMappableDoseAccessorTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncreaseX.dcm")
ADD_TEST(InterpolationTest ${INTERPOLATION_TESTS} InterpolationTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncreaseX.dcm")


ADD_SUBDIRECTORY(InterpolationITKTransformation)

IF(BUILD_InterpolationMatchPointTransformation)
	ADD_SUBDIRECTORY(InterpolationMatchPointTransformation)
ENDIF(BUILD_InterpolationMatchPointTransformation)

RTTB_CREATE_TEST_MODULE(rttbInterpolation DEPENDS RTTBInterpolation RTTBDicomIO PACKAGE_DEPENDS Litmus RTTBData)
