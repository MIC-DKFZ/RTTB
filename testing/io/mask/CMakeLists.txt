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

ADD_TEST(MaskAccessorConverterTest ${MaskIO_TEST} MaskAccessorConverterTest 
 "${TEST_DATA_ROOT}/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm"
 "${TEST_DATA_ROOT}/MatchPointLogo.mhd")


RTTB_CREATE_TEST_MODULE(rttbMaskIO DEPENDS RTTBMaskIO RTTBDicomIO RTTBITKIO RTTBMasks PACKAGE_DEPENDS Boost Litmus MatchPoint ITK DCMTK)

