#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(VIRTUOS_TEST_EXAMPLES ${EXECUTABLE_OUTPUT_PATH}/rttbExamplesVirtuosTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/Temporary)


#-----------------------------------------------------------------------------

ADD_TEST(RTDoseStatisticsVirtuosTest ${VIRTUOS_TEST_EXAMPLES} RTDoseStatisticsVirtuosTest
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln")

RTTB_CREATE_TEST_MODULE(rttbExamplesVirtuos DEPENDS RTTBCore RTTBAlgorithms RTTBMasks RTTBBoostMask RTTBIndices RTTBDicomIO RTTBVirtuosIO RTTBITKIO RTTBOtherIO RTTBModels PACKAGE_DEPENDS Litmus)

