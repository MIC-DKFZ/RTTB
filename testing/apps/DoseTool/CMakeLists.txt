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
	SET(DOSETOOL4VEXE "DoseTool4V.exe")
ELSE (WIN32)
	SET(DOSETOOLEXE "./DoseTool")
	SET(DOSETOOL4VEXE "./DoseTool4V")
ENDIF (WIN32)

ADD_TEST(DoseToolBasicUsageTest ${DOSETOOL_TEST} DoseToolBasicUsageTest ${DOSETOOLEXE})
ADD_TEST(DoseToolInvalidParametersTest ${DOSETOOL_TEST} DoseToolInvalidParametersTest ${DOSETOOLEXE})
IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
	ADD_TEST(DoseToolVirtuosDoseVirtuosStructTest ${DOSETOOL_TEST} DoseToolVirtuosDoseTest ${DOSETOOL4VEXE} "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln"
		"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz" "DARM"
		 "${TEST_DATA_ROOT}/DoseStatistics/XML/virtuos.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/virtuosComplex.xml")
	 ADD_TEST(DoseToolITKDoseVirtuosStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest ${DOSETOOL4VEXE} "${TEST_DATA_ROOT}/Dose/ITK/virtuosTestDose.mhd" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz"  "DARM" "${TEST_DATA_ROOT}/DoseStatistics/XML/itkVirtuosStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/itkVirtuosStructComplex.xml")
ENDIF()
ADD_TEST(DoseToolDicomDoseDicomStructTest ${DOSETOOL_TEST} DoseToolDicomDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/XML/dicom.xml" "${TEST_DATA_ROOT}/DVH/XML/dicompylerDVH.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/dicomComplex.xml")
ADD_TEST(DoseToolITKDoseDicomStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/ITK/dicompylerTestDose.mhd" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/XML/itkDicomStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/itkDicomStructComplex.xml")
ADD_TEST(DoseToolDicomDoseDicomStructRegexTest ${DOSETOOL_TEST} DoseToolRegexTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "dicom" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "dicom" "Nodes|Heart"
"${TEST_DATA_ROOT}/DoseStatistics/XML/dicom.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/dicom_heart.xml")
ADD_TEST(DoseToolDVHTest ${DOSETOOL_TEST} DoseToolDVHTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/DVH/XML/dicompylerDVH.xml")

RTTB_CREATE_APPLICATION_TESTS(DoseTool PACKAGE_DEPENDS Litmus BoostBinaries)
