// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>

#include "litCheckMacros.h"
#include "litImageTester.h"

#include "rttbDoseIteratorInterface.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbITKImageAccessorConverter.h"
#include "rttbITKImageFileAccessorGenerator.h"

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

			io::itk::ITKImageAccessorConverter itkConverter(doseAccessor);

			CHECK_NO_THROW(itkConverter.process());
			CHECK_NO_THROW(itkConverter.getITKImage());

			io::itk::ITKImageAccessorConverter::ITKImageType::IndexType itkIndex;
			itkIndex[0] = itkIndex[1] = itkIndex[2] = 0;

			VoxelGridIndex3D rttbIndex(0, 0, 0);

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), doseAccessor->getValueAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() / 2;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() / 2;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() / 2;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), doseAccessor->getValueAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() - 1;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() - 1;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() - 1;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), doseAccessor->getValueAt(rttbIndex));

			//2) Read mhdFile and test getITKImage() with Litmus TestImageIO

			io::itk::ITKImageFileAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			io::itk::ITKImageAccessorConverter itkConverter2(doseAccessor2);

			CHECK_NO_THROW(itkConverter2.process());
			CHECK_NO_THROW(itkConverter2.getITKImage());

			io::itk::ITKImageAccessorConverter::ITKImageType::Pointer expectedImage =
			    lit::TestImageIO<unsigned char, io::itk::ITKImageAccessorConverter::ITKImageType>::readImage(
			        RTDOSE2_FILENAME);

			::lit::ImageTester<io::itk::ITKImageAccessorConverter::ITKImageType, io::itk::ITKImageAccessorConverter::ITKImageType >
			tester;
			tester.setExpectedImage(expectedImage);
			tester.setActualImage(itkConverter2.getITKImage());

			CHECK_TESTER(tester);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

