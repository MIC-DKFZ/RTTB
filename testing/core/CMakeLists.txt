#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(CORE_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbCoreTests)
SET(CORE_HEADER_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbCoreHeaderTest)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(GeometricInfoTest ${CORE_TESTS} GeometricInfoTest)
ADD_TEST(MaskVoxelTest ${CORE_TESTS} MaskVoxelTest)
ADD_TEST(GenericDoseIteratorTest ${CORE_TESTS} GenericDoseIteratorTest)
ADD_TEST(GenericMaskedDoseIteratorTest ${CORE_TESTS} GenericMaskedDoseIteratorTest)
ADD_TEST(DVHCalculatorTest ${CORE_TESTS} DVHCalculatorTest)
ADD_TEST(DVHTest ${CORE_TESTS} DVHTest)
ADD_TEST(DVHSetTest ${CORE_TESTS} DVHSetTest)
ADD_TEST(StructureTest ${CORE_TESTS} StructureTest)
ADD_TEST(StrVectorStructureSetGeneratorTest ${CORE_TESTS} StrVectorStructureSetGeneratorTest)


RTTB_CREATE_TEST_MODULE(rttbCore DEPENDS RTTBCore PACKAGE_DEPENDS Boost Litmus)

