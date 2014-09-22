#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(ITKIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbITKIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(ITKDoseAccessorGeneratorTest ${ITKIO_TEST} ITKDoseAccessorGeneratorTest
"${TEST_DATA_ROOT}/MatchPointLogo.mhd") 

ADD_TEST(ITKIOTest ${ITKIO_TEST} ITKIOTest 
"${TEST_DATA_ROOT}/MatchPointLogo.mhd")

ADD_TEST(ITKDoseAccessorConverterTest ${ITKIO_TEST} ITKDoseAccessorConverterTest 
 "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/MatchPointLogo.mhd" )


RTTB_CREATE_TEST_MODULE(rttbITKIO DEPENDS RTTBITKIO RTTBDicomIO PACKAGE_DEPENDS Boost Litmus MatchPoint ITK DCMTK)

