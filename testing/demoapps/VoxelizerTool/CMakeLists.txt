#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(CORE_TEST_EXAMPLES ${EXECUTABLE_OUTPUT_PATH}/rttbExamplesTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(rttbVoxelizerToolDifferentCommandsTest ${CORE_TEST_EXAMPLES} RTBioModelExampleTest

ADD_TEST(rttbVoxelizerToolIncorrectCommandsTest ${CORE_TEST_EXAMPLES} DVHCalculatorExampleTest 

ADD_TEST(rttbVoxelizerToolVoxelizerAllStuctsTest ${CORE_TEST_EXAMPLES} RTDoseIndexTest 

ADD_TEST(rttbVoxelizerToolVoxelizerBoostLegacyTest ${CORE_TEST_EXAMPLES} RTDoseStatisticsTest 

ADD_TEST(rttbVoxelizerToolVoxelPixelValueTest ${CORE_TEST_EXAMPLES} RTDVHTest "${TEST_DATA_ROOT}/TestDVH/dvh_test.txt")

RTTB_CREATE_TEST_MODULE(rttbDemoappsVoxelizertool DEPENDS RTTBCore RTTBAlgorithms RTTBMasks RTTBOTBMask RTTBBoostMask RTTBIndices RTTBDicomIO RTTBVirtuosIO RTTBOtherIO RTTBModels PACKAGE_DEPENDS Litmus)



	
	
	
	