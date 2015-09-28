#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(VoxelizerTool_TESTS ${EXECUTABLE_OUTPUT_PATH}/RTTBVoxelizerToolTests)
SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)
SET(TEST_DATA ${RTToolbox_BINARY_DIR}/Testing/demoapps/VoxelizerTool)

SET(VOXELIZERTOOL_EXE "VoxelizerTool")
SET(STRUCT_FILE "${TEST_DATA_ROOT}/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm")
SET(REFERENCE_FILE "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm")

#-----------------------------------------------------------------------------
ADD_TEST(rttbVoxelizerToolIncorrectCommandsTest ${VoxelizerTool_TESTS} VoxelizerToolIncorrectCommandsTest 
"${VOXELIZERTOOL_EXE}" ${STRUCT_FILE} "${TEST_DATA_ROOT}/DICOM/StructureSet/Wrong_Data_Struct_file.dicom" 
${REFERENCE_FILE} "${TEST_DATA_ROOT}/DICOM/TestDose/Wrong_Reference_file.dicom" "Rueckenmark" "blabla")
ADD_TEST(rttbVoxelizerToolVoxelizerAllStructsTest ${VoxelizerTool_TESTS} VoxelizerToolVoxelizerAllStructsTest 
"${VOXELIZERTOOL_EXE}" ${TEST_DATA} ${STRUCT_FILE} ${REFERENCE_FILE} )
ADD_TEST(rttbVoxelizerToolDifferentCommandsTest ${VoxelizerTool_TESTS} VoxelizerToolDifferentCommandsTest 
"${VOXELIZERTOOL_EXE}" ${TEST_DATA} ${STRUCT_FILE} ${REFERENCE_FILE})
ADD_TEST(rttbVoxelizerToolVoxelizerBoostLegacyTest ${VoxelizerTool_TESTS} VoxelizerToolVoxelizerBoostLegacy 
"${VOXELIZERTOOL_EXE}" ${TEST_DATA} ${STRUCT_FILE} ${REFERENCE_FILE})
ADD_TEST(rttbVoxelizerToolVoxelValueTest  ${VoxelizerTool_TESTS} VoxelizerToolVoxelValue 
"${VOXELIZERTOOL_EXE}" ${TEST_DATA} ${STRUCT_FILE} ${REFERENCE_FILE} "Leber")

RTTB_CREATE_TEST_MODULE(RTTBVoxelizerTool DEPENDS RTTBITKIO RTTBCore RTTBMasks RTTBOTBMask RTTBBoostMask RTTBDicomIO RTTBOtherIO PACKAGE_DEPENDS Litmus)
