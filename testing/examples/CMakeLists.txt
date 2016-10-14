#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(CORE_TEST_EXAMPLES ${EXECUTABLE_OUTPUT_PATH}/rttbExamplesTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/Temporary)


#-----------------------------------------------------------------------------
ADD_TEST(RTBioModelExampleTest ${CORE_TEST_EXAMPLES} RTBioModelExampleTest
"${TEST_DATA_ROOT}/DVH/Text/dvh_PTV_HIT.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT1.txt"
"${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT2.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT3.txt"
"${TEST_DATA_ROOT}/DVH/Text/dvh_test_TV.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_virtuos_diff_trunk6.txt"
"${TEST_DATA_ROOT}/DVH/Text/dvh_virtuos_diff_trunk8.txt")
ADD_TEST(RTDoseIndexTest ${CORE_TEST_EXAMPLES} RTDoseIndexTest
"${TEST_DATA_ROOT}/DVH/Text/dvh_test_TV.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT1.txt"
"${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT2.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT3.txt")
ADD_TEST(RTDoseStatisticsDicomTest ${CORE_TEST_EXAMPLES} RTDoseStatisticsDicomTest
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo_withDoseGridScaling.dcm")
ADD_TEST(RTDVHTest ${CORE_TEST_EXAMPLES} RTDVHTest "${TEST_DATA_ROOT}/DVH/Text/dvh_test.txt")
ADD_TEST(RTBioModelScatterPlotExampleTest ${CORE_TEST_EXAMPLES} RTBioModelScatterPlotExampleTest
"${TEST_DATA_ROOT}/DVH/Text/dvh_PTV_HIT.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT1.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_TV.txt")

RTTB_CREATE_TEST_MODULE(rttbExamples DEPENDS RTTBCore RTTBAlgorithms RTTBMasks RTTBBoostMask RTTBIndices RTTBDicomIO RTTBITKIO RTTBOtherIO RTTBModels PACKAGE_DEPENDS Litmus)