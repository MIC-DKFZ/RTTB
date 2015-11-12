#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(DOSETOOL_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbDoseToolTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

IF(MSVC)
     ADD_DEFINITIONS(/bigobj)
ENDIF()

ADD_TEST(DoseToolBasicUsageTest ${DOSETOOL_TEST} DoseToolBasicUsageTest)
ADD_TEST(DoseToolInvalidParametersTest ${DOSETOOL_TEST} DoseToolInvalidParametersTest)
ADD_TEST(DoseToolVirtuosDoseVirtuosStructTest ${DOSETOOL_TEST} DoseToolVirtuosDoseTest "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz" "DARM"
	 "${TEST_DATA_ROOT}/DoseStatistics/virtuos.xml" "${TEST_DATA_ROOT}/DoseStatistics/virtuosComplex.xml")
ADD_TEST(DoseToolDicomDoseDicomStructTest ${DOSETOOL_TEST} DoseToolDicomDoseTest "${TEST_DATA_ROOT}/DICOM/TestDose/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/DICOM/StructureSet/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/dicom.xml" "${TEST_DATA_ROOT}/TestDVH/dicompylerDVH.xml" "${TEST_DATA_ROOT}/DoseStatistics/dicomComplex.xml")
ADD_TEST(DoseToolITKDoseDicomStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest "${TEST_DATA_ROOT}/ITK/dicompylerTestDose.mhd" "${TEST_DATA_ROOT}/DICOM/StructureSet/rtss.dcm" "" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/itkDicomStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/itkDicomStructComplex.xml")
ADD_TEST(DoseToolITKDoseVirtuosStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest "${TEST_DATA_ROOT}/ITK/virtuosTestDose.mhd" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz"  "DARM" "${TEST_DATA_ROOT}/DoseStatistics/itkVirtuosStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/itkVirtuosStructComplex.xml")
ADD_TEST(DoseToolDVHTest ${DOSETOOL_TEST} DoseToolDVHTest "${TEST_DATA_ROOT}/DICOM/TestDose/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/DICOM/StructureSet/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/TestDVH/dicompylerDVH.xml")

RTTB_CREATE_APPLICATION_TESTS(DoseTool PACKAGE_DEPENDS Litmus BoostBinaries)
