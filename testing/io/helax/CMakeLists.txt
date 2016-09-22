#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(HelaxIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbHelaxIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(DicomHelaxDoseAccessorGeneratorTest ${HelaxIO_TEST} DicomHelaxDoseAccessorGeneratorTest
"${TEST_DATA_ROOT}/Dose/DICOM/Helax/" "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantFifty.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm"
 )

ADD_TEST(DicomHelaxIOTest ${HelaxIO_TEST} DicomHelaxIOTest 
"${TEST_DATA_ROOT}/Dose/DICOM/Helax/"
 )

RTTB_CREATE_TEST_MODULE(rttbHelaxIO DEPENDS RTTBHelaxIO RTTBDicomIO PACKAGE_DEPENDS BoostBinaries Litmus DCMTK)

