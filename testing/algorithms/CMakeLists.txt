#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(ALGORITHMS_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbAlgorithmsTests)
SET(ALGORITHMS_HEADER_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbAlgorithmsHeaderTest)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(DoseStatisticsTest ${ALGORITHMS_TESTS} DoseStatisticsTest)
ADD_TEST(ArithmeticTest ${ALGORITHMS_TESTS} ArithmeticTest)
ADD_TEST(BinaryFunctorAccessorTest ${ALGORITHMS_TESTS} BinaryFunctorAccessorTest "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/dicompylerTestDose.dcm")

RTTB_CREATE_TEST_MODULE(rttbAlgorithms DEPENDS RTTBAlgorithms RTTBMasks RTTBDicomIO PACKAGE_DEPENDS Boost Litmus DCMTK)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
