#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(VoxelizerTool_TESTS ${EXECUTABLE_OUTPUT_PATH}/Release/RTTBDemoappsVoxelizertoolTests)
SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)
SET(TEST_DATA ${RTToolbox_BINARY_DIR}/Testing/demoapps/VoxelizerTool)

#-----------------------------------------------------------------------------
ADD_TEST(rttbVoxelizerToolIncorrectCommandsTest ${VoxelizerTool_TESTS} VoxelizerToolIncorrectCommandsTest 
${EXECUTABLE_OUTPUT_PATH} ${TEST_DATA} ${TEST_DATA_ROOT})
ADD_TEST(rttbVoxelizerToolVoxelizerAllStuctsTest ${VoxelizerTool_TESTS} VoxelizerToolVoxelizerAllStructsTest 
${EXECUTABLE_OUTPUT_PATH} ${TEST_DATA} ${TEST_DATA_ROOT})
ADD_TEST(rttbVoxelizerToolDifferentCommandsTest ${VoxelizerTool_TESTS} VoxelizerToolDifferentCommandsTest 
${EXECUTABLE_OUTPUT_PATH} ${TEST_DATA} ${TEST_DATA_ROOT})
ADD_TEST(rttbVoxelizerToolVoxelizerBoostLegacyTest ${VoxelizerTool_TESTS} VoxelizerToolVoxelizerBoostLegacy 
${EXECUTABLE_OUTPUT_PATH} ${TEST_DATA} ${TEST_DATA_ROOT})
ADD_TEST(rttbVoxelizerToolVoxelPixelValueTest  ${VoxelizerTool_TESTS} VoxelizerToolVoxelPixelValue 
${EXECUTABLE_OUTPUT_PATH} ${TEST_DATA} ${TEST_DATA_ROOT})

RTTB_CREATE_TEST_MODULE(RTTBDemoappsVoxelizertool DEPENDS RTTBITKIO RTTBCore RTTBAlgorithms RTTBMasks RTTBOTBMask RTTBBoostMask RTTBIndices RTTBDicomIO RTTBOtherIO PACKAGE_DEPENDS Litmus)
