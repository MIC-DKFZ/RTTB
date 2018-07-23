#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(CORE_TEST_EXAMPLES ${EXECUTABLE_OUTPUT_PATH}/rttbExamplesTests)
SET(TEMP ${RTTBTesting_BINARY_DIR}/Temporary)


#-----------------------------------------------------------------------------
ADD_TEST(RTBioModelExampleTest ${CORE_TEST_EXAMPLES} RTBioModelExampleTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_PTV_HIT.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT2.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT3.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_virtuos_diff_trunk6.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_virtuos_diff_trunk8.xml")
ADD_TEST(RTDoseIndexTest ${CORE_TEST_EXAMPLES} RTDoseIndexTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml"
"${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT2.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT3.xml")
ADD_TEST(RTDoseStatisticsDicomTest ${CORE_TEST_EXAMPLES} RTDoseStatisticsDicomTest
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo_withDoseGridScaling.dcm")
ADD_TEST(RTDVHTest ${CORE_TEST_EXAMPLES} RTDVHTest "${TEST_DATA_ROOT}/DVH/XML/dvh_test.xml")
ADD_TEST(RTBioModelScatterPlotExampleTest ${CORE_TEST_EXAMPLES} RTBioModelScatterPlotExampleTest
"${TEST_DATA_ROOT}/DVH/XML/dvh_PTV_HIT.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_HT1.xml" "${TEST_DATA_ROOT}/DVH/XML/dvh_test_TV.xml")

RTTB_CREATE_TEST_MODULE(Examples DEPENDS RTTBCore RTTBAlgorithms RTTBMasks RTTBBoostMask RTTBIndices RTTBDicomIO RTTBITKIO RTTBOtherIO RTTBModels PACKAGE_DEPENDS Litmus RTTBData)
