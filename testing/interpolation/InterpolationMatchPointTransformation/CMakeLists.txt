#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(INTERPOLATION_MP_TRANSFORMATION_TESTS ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}InterpolationMPTransformationTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)

#-----------------------------------------------------------------------------
ADD_TEST(SimpleMappableDoseAccessorWithMatchPointTest ${INTERPOLATION_MP_TRANSFORMATION_TESTS} SimpleMappableDoseAccessorWithMatchPointTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncreaseX.dcm")

RTTB_CREATE_TEST_MODULE(InterpolationMPTransformation DEPENDS RTTBInterpolation RTTBInterpolationMatchPointTransformation RTTBDicomIO PACKAGE_DEPENDS Litmus MatchPoint RTTBData ITK)

