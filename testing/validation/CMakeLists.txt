#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(CORE_TEST_VALIDATION ${EXECUTABLE_OUTPUT_PATH}/rttbValidationTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/Temporary)


#-----------------------------------------------------------------------------

ADD_TEST(VoxelizationDVHComparisonTest ${CORE_TEST_VALIDATION} VoxelizationDVHComparisonTest
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" 
"${TEST_DATA_ROOT}/DVH/XML/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/OTBMask/" 
"${TEST_DATA_ROOT}/DVH/XML/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/BoostMask/" 
"${RTTBTesting_BINARY_DIR}/validation/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/BoostMaskRedesign/")
ADD_TEST(VoxelizationValidationTest ${CORE_TEST_VALIDATION} VoxelizationValidationTest
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" "${TEST_DATA_ROOT}/Mask/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/BoostMask/" 
"${TEST_DATA_ROOT}/Mask/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/OTBMask/" "${RTTBTesting_BINARY_DIR}/validation/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/BoostMaskRedesign/"
"${TEST_DATA_ROOT}/Dose/DICOM/PatBM116__RTDOSE_Main_corr.mhd" "${TEST_DATA_ROOT}/StructureSet/DICOM/__1__mm__1.2.276.0.28.19.640188827224102750711261838091512821903075578763.4195312189")

RTTB_CREATE_TEST_MODULE(rttbValidation DEPENDS RTTBCore RTTBBoostMask RTTBDicomIO RTTBITKIO RTTBOtherIO PACKAGE_DEPENDS Litmus ITK)

