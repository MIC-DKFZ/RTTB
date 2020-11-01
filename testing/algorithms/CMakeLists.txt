#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(ALGORITHMS_TESTS ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}AlgorithmsTests)
SET(ALGORITHMS_HEADER_TEST ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}AlgorithmsHeaderTest)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(DoseStatisticsTest ${ALGORITHMS_TESTS} DoseStatisticsTest)
ADD_TEST(ArithmeticTest ${ALGORITHMS_TESTS} ArithmeticTest)
ADD_TEST(DoseStatisticsCalculatorTest ${ALGORITHMS_TESTS} DoseStatisticsCalculatorTest "${TEST_DATA_ROOT}/DoseStatistics/XML/dicom_heartComplex.xml" "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm")
ADD_TEST(BinaryFunctorAccessorTest ${ALGORITHMS_TESTS} BinaryFunctorAccessorTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm")

RTTB_CREATE_TEST_MODULE(Algorithms DEPENDS RTTBAlgorithms RTTBTestHelper RTTBMask RTTBDicomIO PACKAGE_DEPENDS Boost Litmus RTTBData DCMTK)

