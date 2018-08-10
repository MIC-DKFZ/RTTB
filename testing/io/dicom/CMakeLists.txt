#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(DICOMIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbDicomIOTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(DicomDoseAccessorConverterTest ${DICOMIO_TEST} DicomDoseAccessorConverterTest
"${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm"
"${TEMP}/dose_w.dcm" ) 

ADD_TEST(DicomDoseAccessorGeneratorTest ${DICOMIO_TEST} DicomDoseAccessorGeneratorTest
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantFifty.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" ) 

ADD_TEST(DicomFileReaderHelperTest ${DICOMIO_TEST} DicomFileReaderHelperTest
"${TEST_DATA_ROOT}/Dose/DICOM/Helax/"
"${TEST_DATA_ROOT}/Dose/DICOM/Helax/____mm_1_1.2.276.0.28.19.977891832855880720695789165493875543457754809556.dcm" 
"${TEST_DATA_ROOT}/Dose/DICOM/Helax/____mm__1.2.276.0.28.19.142087956198378746376227895256244905653791675016.dcm"
) 


ADD_TEST(DicomIOTest ${DICOMIO_TEST} DicomIOTest 
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/ConstantFifty.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/LinearIncrease3D.dcm" "${TEST_DATA_ROOT}/Dose/DICOM/dicompylerTestDose.dcm"
"${TEST_DATA_ROOT}/Dose/DICOM/InhomogeneousGrid.dcm" )

ADD_TEST(DicomStructureSetGeneratorTest ${DICOMIO_TEST} DicomStructureSetGeneratorTest 
"${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" )


RTTB_CREATE_TEST_MODULE(DicomIO DEPENDS RTTBDicomIO PACKAGE_DEPENDS Boost Litmus RTTBData DCMTK)

