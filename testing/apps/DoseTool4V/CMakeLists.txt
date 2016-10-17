#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(DOSETOOL4V_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbDoseTool4VTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

IF(MSVC)
     ADD_DEFINITIONS(/bigobj)
ENDIF()

IF (WIN32)
	SET(DOSETOOL4VEXE "DoseTool4V.exe")
ELSE (WIN32)
	SET(DOSETOOL4VEXE "./DoseTool4V")
ENDIF (WIN32)

ADD_TEST(DoseTool4VVirtuosDoseVirtuosStructTest ${DOSETOOL4V_TEST} DoseTool4VVirtuosDoseTest ${DOSETOOL4VEXE} "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln"
		"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz" "DARM"
		 "${TEST_DATA_ROOT}/DoseStatistics/XML/virtuos.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/virtuosComplex.xml")
ADD_TEST(DoseTool4VITKDoseVirtuosStructTest ${DOSETOOL4V_TEST} DoseTool4VITKDoseTest ${DOSETOOL4VEXE} "${TEST_DATA_ROOT}/Dose/ITK/virtuosTestDose.mhd" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx"
	"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz"  "DARM" "${TEST_DATA_ROOT}/DoseStatistics/XML/itkVirtuosStruct.xml" "${TEST_DATA_ROOT}/DoseStatistics/XML/itkVirtuosStructComplex.xml")

RTTB_CREATE_APPLICATION_TESTS(DoseTool4V DEPENDS RTTBOtherIO PACKAGE_DEPENDS Litmus BoostBinaries)
