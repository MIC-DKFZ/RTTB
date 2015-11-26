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

IF (WIN32)
	SET(DOSETOOLEXE "DoseTool.exe")
ELSE (WIN32)
	SET(DOSETOOLEXE "./DoseTool")
ENDIF (WIN32)

ADD_TEST(DoseToolBasicUsageTest ${DOSETOOL_TEST} DoseToolBasicUsageTest ${DOSETOOLEXE})
ADD_TEST(DoseToolInvalidParametersTest ${DOSETOOL_TEST} DoseToolInvalidParametersTest ${DOSETOOLEXE})
ADD_TEST(DoseToolVirtuosDoseVirtuosStructTest ${DOSETOOL_TEST} DoseToolVirtuosDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz" "DARM"
	 "${TEST_DATA_ROOT}/DoseStatistics/virtuos.xml" "${TEST_DATA_ROOT}/DoseStatistics/virtuosComplex.xml")
ADD_TEST(DoseToolDicomDoseDicomStructTest ${DOSETOOL_TEST} DoseToolDicomDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/DICOM/TestDose/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/DICOM/StructureSet/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/dicom.xml" "${TEST_DATA_ROOT}/DoseStatistics/dicomComplex.xml")
ADD_TEST(DoseToolITKDoseDicomStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/ITK/dicompylerTestDose.mhd" "${TEST_DATA_ROOT}/DICOM/StructureSet/rtss.dcm" "" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/itkDicomStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/itkDicomStructComplex.xml")
ADD_TEST(DoseToolITKDoseVirtuosStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/ITK/virtuosTestDose.mhd" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz"  "DARM" "${TEST_DATA_ROOT}/DoseStatistics/itkVirtuosStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/itkVirtuosStructComplex.xml")
ADD_TEST(DoseToolDicomDoseDicomStructRegexTest ${DOSETOOL_TEST} DoseToolRegexTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/DICOM/TestDose/dicompylerTestDose.dcm" "dicom" "${TEST_DATA_ROOT}/DICOM/StructureSet/rtss.dcm" "dicom" "Nodes|Heart"
"${TEST_DATA_ROOT}/DoseStatistics/dicom.xml" "${TEST_DATA_ROOT}/DoseStatistics/dicom_heart.xml")

RTTB_CREATE_APPLICATION_TESTS(DoseTool PACKAGE_DEPENDS Litmus BoostBinaries)
