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

IF(BUILD_ITKBinding)
	ADD_SUBDIRECTORY(ITKBinding)
ENDIF(BUILD_ITKBinding)
IF(BUILD_MatchPointBinding)
	ADD_SUBDIRECTORY(MatchPointBinding)
ENDIF(BUILD_MatchPointBinding)

RTTB_CREATE_TEST_MODULE(rttbInterpolation DEPENDS RTTBInterpolation RTTBDicomIO PACKAGE_DEPENDS Litmus)
IF (NOT WIN32)
	#Minimum CMake Version is 2.8.11. CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
