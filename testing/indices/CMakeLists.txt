#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(INDICES_TESTS ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}IndicesTests)
SET(INDICES_HEADER_TEST ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}CoreIndicesTest)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)

#-----------------------------------------------------------------------------
ADD_TEST(ConformationNumberTest ${INDICES_TESTS} ConformationNumberTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT2.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT3.xml")
ADD_TEST(ConformalIndexTest ${INDICES_TESTS} ConformalIndexTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT2.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT3.xml")
ADD_TEST(ConformityIndexTest ${INDICES_TESTS} ConformityIndexTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT2.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT3.xml")
ADD_TEST(CoverageIndexTest ${INDICES_TESTS} CoverageIndexTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT2.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT3.xml")
ADD_TEST(HomogeneityIndexTest ${INDICES_TESTS} HomogeneityIndexTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT2.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT3.xml")
ADD_TEST(GammaIndexTest ${INDICES_TESTS} GammaIndexTest)

RTTB_CREATE_TEST_MODULE(Indices DEPENDS RTTBCore RTTBIndices RTTBTestHelper RTTBOtherIO PACKAGE_DEPENDS Boost Litmus)



