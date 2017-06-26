#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(ITKIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbITKIOTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(ITKDoseAccessorGeneratorTest ${ITKIO_TEST} ITKDoseAccessorGeneratorTest "${TEST_DATA_ROOT}/Images/ITK/MatchPointLogoSmall.nrrd")

ADD_TEST(ITKIOTest ${ITKIO_TEST} ITKIOTest "${TEST_DATA_ROOT}/Images/ITK/MatchPointLogoSmall.nrrd")

ADD_TEST(ITKDoseAccessorConverterTest ${ITKIO_TEST} ITKDoseAccessorConverterTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/Images/ITK/MatchPointLogoSmall.nrrd" )

 ADD_TEST(ITKBioModelAccessorConverterTest ${ITKIO_TEST} ITKBioModelAccessorConverterTest "${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm")

ADD_TEST(ITKMaskAccessorGeneratorTest ${ITKIO_TEST} ITKMaskAccessorGeneratorTest "${TEST_DATA_ROOT}/StructureSet/ITK/Nodes.nrrd")

ADD_TEST(ITKMaskAccessorConverterTest ${ITKIO_TEST} ITKMaskAccessorConverterTest "${TEST_DATA_ROOT}/StructureSet/DICOM/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm" 
	"${TEST_DATA_ROOT}/Dose/DICOM/ConstantTwo.dcm" "${TEST_DATA_ROOT}/StructureSet/ITK/Nodes.nrrd")


RTTB_CREATE_TEST_MODULE(rttbITKIO DEPENDS RTTBITKIO RTTBDicomIO RTTBMasks RTTBBoostMask RTTBModels PACKAGE_DEPENDS Boost Litmus ITK DCMTK RTTBData)

