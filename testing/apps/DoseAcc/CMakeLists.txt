#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(DOSEACC_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbDoseAccTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

IF(MSVC)
     ADD_DEFINITIONS(/bigobj)
ENDIF()

IF (WIN32)
	SET(DOSEACCEXE "DoseAcc.exe")
ELSE (WIN32)
	SET(DOSEACCEXE "./DoseAcc")
ENDIF (WIN32)

ADD_TEST(DoseAccInvalidParametersTest ${DOSEACC_TEST} DoseAccInvalidParametersTest ${DOSEACCEXE})
ADD_TEST(DoseAccSimpleTest ${DOSEACC_TEST} DoseAccSimpleTest ${DOSEACCEXE} "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantFifty.dcm" "${TEST_DATA_ROOT}/ITK/doseAccReference.nrrd") 

RTTB_CREATE_APPLICATION_TESTS(DoseAcc DEPENDS RTTBITKIO PACKAGE_DEPENDS Litmus ITK BoostBinaries)