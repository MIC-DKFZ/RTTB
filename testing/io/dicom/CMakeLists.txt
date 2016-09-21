#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(DICOMIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbDicomIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(DicomDoseAccessorConverterTest ${DICOMIO_TEST} DicomDoseAccessorConverterTest
"${TEST_DATA_ROOT}/DICOM/Dose/LinearIncrease3D.dcm"
"${TEST_DATA_ROOT}/DICOM/Dose/dose_w.dcm" ) 

ADD_TEST(DicomDoseAccessorGeneratorTest ${DICOMIO_TEST} DicomDoseAccessorGeneratorTest
"${TEST_DATA_ROOT}/DICOM/Dose/ConstantTwo.dcm"
"${TEST_DATA_ROOT}/DICOM/Dose/ConstantFifty.dcm" "${TEST_DATA_ROOT}/DICOM/Dose/LinearIncrease3D.dcm" ) 

ADD_TEST(DicomFileReaderHelperTest ${DICOMIO_TEST} DicomFileReaderHelperTest
"${TEST_DATA_ROOT}/DICOM/Helax/"
"${TEST_DATA_ROOT}/DICOM/Helax/____mm_1_1.2.276.0.28.19.977891832855880720695789165493875543457754809556.dcm" 
"${TEST_DATA_ROOT}/DICOM/Helax/____mm__1.2.276.0.28.19.142087956198378746376227895256244905653791675016.dcm"
) 


ADD_TEST(DicomIOTest ${DICOMIO_TEST} DicomIOTest 
"${TEST_DATA_ROOT}/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/DICOM/Dose/ConstantTwo.dcm"
"${TEST_DATA_ROOT}/DICOM/Dose/ConstantFifty.dcm" "${TEST_DATA_ROOT}/DICOM/Dose/LinearIncrease3D.dcm" "${TEST_DATA_ROOT}/DICOM/Dose/dicompylerTestDose.dcm"
"${TEST_DATA_ROOT}/DICOM/Dose/InhomogeneousGrid.dcm" )

ADD_TEST(DicomStructureSetGeneratorTest ${DICOMIO_TEST} DicomStructureSetGeneratorTest 
"${TEST_DATA_ROOT}/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" )


RTTB_CREATE_TEST_MODULE(rttbDicomIO DEPENDS RTTBDicomIO PACKAGE_DEPENDS Boost Litmus DCMTK)

