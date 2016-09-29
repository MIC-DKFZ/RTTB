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
ADD_TEST(DVHCalculatorExampleTest ${CORE_TEST_EXAMPLES} DVHCalculatorExampleTest
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwoGridScaling.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwoGridScaling05.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/ConstantFiftyGridScaling01.dcm")
ADD_TEST(RTDoseIndexTest ${CORE_TEST_EXAMPLES} RTDoseIndexTest
"${TEST_DATA_ROOT}/DVH/Text/dvh_test_TV.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT1.txt"
"${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT2.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT3.txt")
ADD_TEST(RTDoseStatisticsDicomTest ${CORE_TEST_EXAMPLES} RTDoseStatisticsDicomTest
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo_withDoseGridScaling.dcm")
IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
	ADD_TEST(RTDoseStatisticsVirtuosTest ${CORE_TEST_EXAMPLES} RTDoseStatisticsVirtuosTest
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln")
ENDIF()
ADD_TEST(RTDVHTest ${CORE_TEST_EXAMPLES} RTDVHTest "${TEST_DATA_ROOT}/DVH/Text/dvh_test.txt")
ADD_TEST(DVHCalculatorExampleTest ${CORE_TEST_EXAMPLES} DVHCalculatorExampleTest
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo_withDoseGridScaling.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncreaseX.dcm")
ADD_TEST(RTBioModelScatterPlotExampleTest ${CORE_TEST_EXAMPLES} RTBioModelScatterPlotExampleTest
"${TEST_DATA_ROOT}/DVH/Text/dvh_PTV_HIT.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_HT1.txt" "${TEST_DATA_ROOT}/DVH/Text/dvh_test_TV.txt")
ADD_TEST(VoxelizationValidationTest ${CORE_TEST_EXAMPLES} VoxelizationValidationTest
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" "${TEST_DATA_ROOT}/Mask/BoostMask/" "${TEST_DATA_ROOT}/Mask/OTBMask/" "${TEST_DATA_ROOT}/Mask/BoostMaskRedesign/"
"${TEST_DATA_ROOT}/Dose/DICOM/PatBM116__RTDOSE_Main_corr.mhd" "${TEST_DATA_ROOT}/StructureSet/DICOM/__1__mm__1.2.276.0.28.19.640188827224102750711261838091512821903075578763.4195312189")

IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
	RTTB_CREATE_TEST_MODULE(rttbExamples DEPENDS RTTBCore RTTBAlgorithms RTTBMasks RTTBOTBMask RTTBBoostMask RTTBIndices RTTBDicomIO RTTBVirtuosIO RTTBITKIO RTTBOtherIO RTTBModels PACKAGE_DEPENDS Litmus)
ELSE()
	RTTB_CREATE_TEST_MODULE(rttbExamples DEPENDS RTTBCore RTTBAlgorithms RTTBMasks RTTBOTBMask RTTBBoostMask RTTBIndices RTTBDicomIO RTTBITKIO RTTBOtherIO RTTBModels PACKAGE_DEPENDS Litmus)
ENDIF()
