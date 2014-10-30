#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(DICOMIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbDicomIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(DicomDoseAccessorGeneratorTest ${DICOMIO_TEST} DicomDoseAccessorGeneratorTest
"${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm"
"${TEST_DATA_ROOT}/DICOM/TestDose/ConstantFifty.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncrease3D.dcm" ) 

ADD_TEST(DicomIOTest ${DICOMIO_TEST} DicomIOTest 
"${TEST_DATA_ROOT}/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/ConstantTwo.dcm"
"${TEST_DATA_ROOT}/DICOM/TestDose/ConstantFifty.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/LinearIncrease3D.dcm" "${TEST_DATA_ROOT}/DICOM/TestDose/dicompylerTestDose.dcm"
"${TEST_DATA_ROOT}/DICOM/TestDose/InhomogeneousGrid.dcm" )

ADD_TEST(DicomStructureSetGeneratorTest ${DICOMIO_TEST} DicomStructureSetGeneratorTest 
"${TEST_DATA_ROOT}/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" )


RTTB_CREATE_TEST_MODULE(rttbDicomIO DEPENDS RTTBDicomIO PACKAGE_DEPENDS Boost Litmus DCMTK)

