#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(MaskIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbMaskIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(MaskAccessorGeneratorTest ${MaskIO_TEST} MaskAccessorGeneratorTest
"${TEST_DATA_ROOT}/MatchPointLogo.mhd") 

ADD_TEST(MaskIOTest ${MaskIO_TEST} MaskIOTest 
"${TEST_DATA_ROOT}/MatchPointLogo.mhd")

ADD_TEST(MaskAccessorConverterTest ${MaskIO_TEST} MaskAccessorConverterTest 
 "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm" "${TEST_DATA_ROOT}/MatchPointLogo.mhd" )


RTTB_CREATE_TEST_MODULE(rttbMaskIO DEPENDS RTTBMaskIO RTTBDicomIO RTTBITKIO PACKAGE_DEPENDS Boost Litmus MatchPoint ITK DCMTK)

