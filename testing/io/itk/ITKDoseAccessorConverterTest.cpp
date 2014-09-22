// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// (c) Copyright 2007, DKFZ, Heidelberg, Germany
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR
// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author zhangl (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/trunk/testing/core/DVHCalculatorTest.cpp $
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"
#include "litImageTester.h"
#include "litTestImageIO.h"

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "rttbBaseType.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbITKImageDoseAccessor.h"
#include "rttbITKImageFileDoseAccessorGenerator.h"
#include "rttbITKImageDoseAccessorGenerator.h"
#include "rttbITKImageDoseAccessorConverter.h"
#include "rttbDoseAccessorProcessorBase.h"
#include "rttbDoseAccessorConversionSettingInterface.h"
#include "rttbInvalidDoseException.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief ITKDoseAccessorConverterTest - test the conversion rttb dose accessor ->itk
		1) test with dicom file (DicomDoseAccessorGenerator)
		2) test with mhd file (ITKImageFileDoseAccessorGenerator)
		*/

		int ITKDoseAccessorConverterTest(int argc, char* argv[])
		{
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			//           1: dose1 file name
			//           2: dose2 file name

			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE2_FILENAME = argv[2];
			}

			//1) Read dicomFile and test getITKImage()

			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor(doseAccessorGenerator.generateDoseAccessor());

			io::itk::ITKImageDoseAccessorConverter itkConverter(doseAccessor);

			CHECK_NO_THROW(itkConverter.process());
			CHECK_NO_THROW(itkConverter.getITKImage());

			io::itk::ITKDoseImageType::IndexType itkIndex;
			itkIndex[0] = itkIndex[1] = itkIndex[2] = 0;

			VoxelGridIndex3D rttbIndex(0, 0, 0);

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), doseAccessor->getDoseAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() / 2;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() / 2;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() / 2;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), doseAccessor->getDoseAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() - 1;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() - 1;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() - 1;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), doseAccessor->getDoseAt(rttbIndex));

			//2) Read mhdFile and test getITKImage() with Litmus TestImageIO

			io::itk::ITKImageFileDoseAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			io::itk::ITKImageDoseAccessorConverter itkConverter2(doseAccessor2);

			CHECK_NO_THROW(itkConverter2.process());
			CHECK_NO_THROW(itkConverter2.getITKImage());

			io::itk::ITKDoseImageType::Pointer expectedImage =
			    lit::TestImageIO<unsigned char, io::itk::ITKDoseImageType>::readImage(
			        RTDOSE2_FILENAME);

			::lit::ImageTester<io::itk::ITKDoseImageType, io::itk::ITKDoseImageType >
			tester;
			tester.setExpectedImage(expectedImage);
			tester.setActualImage(itkConverter2.getITKImage());

			CHECK_TESTER(tester);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

