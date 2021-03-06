#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(BIOMODELCALC_TEST ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}BioModelCalcTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

IF(MSVC)
     ADD_DEFINITIONS(/bigobj)
ENDIF()

IF (WIN32)
	SET(BIOMODELCALCEXE "BioModelCalc.exe")
ELSE (WIN32)
	SET(BIOMODELCALCEXE "./BioModelCalc")
ENDIF (WIN32)

ADD_TEST(BioModelCalcInvalidParametersTest ${BIOMODELCALC_TEST} BioModelCalcInvalidParametersTest ${BIOMODELCALCEXE})
ADD_TEST(BioModelCalcSimpleTest ${BIOMODELCALC_TEST} BioModelCalcSimpleTest ${BIOMODELCALCEXE} "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "dicom" "LQ" "0.1 0.01"
"${TEST_DATA_ROOT}/BioModel/Images/simpleReference.nrrd" "${TEST_DATA_ROOT}/BioModel/Images/simpleReferenceWithNFractions.nrrd")
ADD_TEST(BioModelCalcParameterMapsTest ${BIOMODELCALC_TEST} BioModelCalcParameterMapsTest ${BIOMODELCALCEXE} "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "dicom" "LQ" 
	"${TEST_DATA_ROOT}/Dose/ITK/constant01.nrrd ${TEST_DATA_ROOT}/Dose/ITK/constant001.nrrd" "itk" "${TEST_DATA_ROOT}/BioModel/Images/parameterMapsReference.nrrd")

RTTB_CREATE_APPLICATION_TESTS(BioModelCalc DEPENDS RTTBITKIO PACKAGE_DEPENDS Litmus ITK BoostBinaries RTTBData)
