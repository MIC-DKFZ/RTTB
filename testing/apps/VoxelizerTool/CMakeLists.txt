#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(VoxelizerTool_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbVoxelizerToolTests)
SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)
SET(TEST_DATA ${RTToolbox_BINARY_DIR}/Testing/apps/VoxelizerTool)

IF (WIN32)
	SET(VOXELIZERTOOL_EXE "VoxelizerTool")
ELSE (WIN32)
	SET(VOXELIZERTOOL_EXE "./VoxelizerTool")
ENDIF (WIN32)

SET(STRUCT_FILE "${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm")
SET(REFERENCE_FILE "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm")

#-----------------------------------------------------------------------------
ADD_TEST(VoxelizerToolIncorrectCommandsTest ${VoxelizerTool_TESTS} VoxelizerToolIncorrectCommandsTest 
"${VOXELIZERTOOL_EXE}" ${STRUCT_FILE} "${TEST_DATA_ROOT}/StructureSet/DICOM/Wrong_Data_Struct_file.dicom" 
${REFERENCE_FILE} "${TEST_DATA_ROOT}/Dose/DICOM/Wrong_Reference_file.dicom" "Rueckenmark" "blabla")
ADD_TEST(VoxelizerToolVoxelizerAllStructsTest ${VoxelizerTool_TESTS} VoxelizerToolVoxelizerAllStructsTest 
"${VOXELIZERTOOL_EXE}" ${TEST_DATA} ${STRUCT_FILE} ${REFERENCE_FILE} )
ADD_TEST(VoxelizerToolDifferentCommandsTest ${VoxelizerTool_TESTS} VoxelizerToolDifferentCommandsTest 
"${VOXELIZERTOOL_EXE}" ${TEST_DATA} ${STRUCT_FILE} ${REFERENCE_FILE})
ADD_TEST(VoxelizerToolVoxelValueTest  ${VoxelizerTool_TESTS} VoxelizerToolVoxelValue 
"${VOXELIZERTOOL_EXE}" ${TEST_DATA} ${STRUCT_FILE} ${REFERENCE_FILE} "Leber")

RTTB_CREATE_APPLICATION_TESTS(VoxelizerTool PACKAGE_DEPENDS Litmus ITK BoostBinaries)
