#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(DOSETOOL_TEST ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}DoseToolTests)

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

ADD_TEST(DoseToolInvalidParametersTest ${DOSETOOL_TEST} DoseToolInvalidParametersTest ${DOSETOOLEXE})
ADD_TEST(DoseToolDicomDoseDicomStructTest ${DOSETOOL_TEST} DoseToolDicomDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/XML/dicom.xml" "${TEST_DATA_ROOT}/DVH/XML/dicompylerDVH.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/dicomComplex.xml")
ADD_TEST(DoseToolDicomDoseVoxelizedStructTest ${DOSETOOL_TEST} DoseToolDicomDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/StructureSet/ITK/voxelizedNodes.nrrd" ""
"${TEST_DATA_ROOT}/DoseStatistics/XML/dicom.xml" "${TEST_DATA_ROOT}/DVH/XML/dicompylerDVH_voxelizedITK.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/dicomComplex.xml")
ADD_TEST(DoseToolITKDoseDicomStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/ITK/dicompylerTestDose.mhd" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/XML/itkDicomStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/itkDicomStructComplex.xml" "dicomOutput.xml" "dicomOutputComplex.xml")
ADD_TEST(DoseToolITKDoseITKStructTest ${DOSETOOL_TEST} DoseToolITKDoseTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/ITK/dicompylerTestDose.mhd" "${TEST_DATA_ROOT}/StructureSet/ITK/voxelizedNodes.nrrd" "Nodes"
"${TEST_DATA_ROOT}/DoseStatistics/XML/itkDicomStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/itkDicomStructComplex.xml" "itkOutput.xml" "itkOutputComplex.xml")
ADD_TEST(DoseToolDicomDoseDicomStructRegexTest ${DOSETOOL_TEST} DoseToolRegexTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "dicom" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "dicom" "Nodes|Heart"
"${TEST_DATA_ROOT}/DoseStatistics/XML/dicom.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/dicom_heart.xml")
ADD_TEST(DoseToolDVHTest ${DOSETOOL_TEST} DoseToolDVHTest ${DOSETOOLEXE} "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm" "${TEST_DATA_ROOT}/StructureSet/DICOM/rtss.dcm" "Nodes"
"${TEST_DATA_ROOT}/DVH/XML/dicompylerDVH.xml")

RTTB_CREATE_APPLICATION_TESTS(DoseTool DEPENDS RTTBOtherIO PACKAGE_DEPENDS Litmus BoostBinaries RTTBData)
