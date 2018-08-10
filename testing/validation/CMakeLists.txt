#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(CORE_TEST_VALIDATION ${EXECUTABLE_OUTPUT_PATH}/rttbValidationTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/Temporary)


#-----------------------------------------------------------------------------

ADD_TEST(VoxelizationDVHComparisonTest ${CORE_TEST_VALIDATION} VoxelizationDVHComparisonTest
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" 
"${TEST_DATA_ROOT}/DVH/XML/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/OTBMask/" 
"${TEST_DATA_ROOT}/DVH/XML/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/BoostMask_LEGACY/" 
"${RTTBTesting_BINARY_DIR}/validation/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/BoostMask/")
ADD_TEST(VoxelizationValidationTest ${CORE_TEST_VALIDATION} VoxelizationValidationTest
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" 
"${TEMP}/Mask/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/BoostMask/" 
"${TEST_DATA_ROOT}/Mask/1.3.6.1.4.1.2452.6.2037938358.1234393433.864109958.30410275/OTBMask/")

RTTB_CREATE_TEST_MODULE(Validation DEPENDS RTTBCore RTTBMask RTTBDicomIO RTTBITKIO RTTBOtherIO PACKAGE_DEPENDS Litmus ITK RTTBData)

IF (CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals")
ENDIF()
