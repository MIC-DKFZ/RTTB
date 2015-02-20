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
/*!
// @file
// @version $Revision: 793 $ (last changed revision)
// @date $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author $Author: hentsch $ (last changed by)
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
#include "rttbInvalidDoseException.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbOTBMaskAccessor.h"
#include "rttbITKImageMaskAccessorConverter.h"
#include "rttbITKImageFileMaskAccessorGenerator.h"



namespace rttb
{

	namespace testing
	{

		/*!@brief MaskAccessorConverterTest - test the conversion rttb dose accessor ->itk
		1) test with dicom file (DicomDoseAccessorGenerator)
		2) test with mhd file (ITKImageFileDoseAccessorGenerator)
		*/

		int MaskAccessorConverterTest(int argc, char* argv[])
		{
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
			typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;
			typedef io::mask::ITKImageMaskAccessor::ITKMaskImageType::Pointer ITKImageTypePointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			//ARGUMENTS: 1: structure file name
			//           2: dose1 file name

			std::string RTStr_FILENAME;
			std::string RTDose_FILENAME;
			std::string Mask_FILENAME;

			if (argc > 1)
			{
				RTStr_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDose_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				Mask_FILENAME = argv[3];
			}
			//1) Read dicomFile and test getITKImage()
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDose_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTStr_FILENAME.c_str()).generateStructureSet();


			MaskAccessorPointer maskAccessorPtr = boost::make_shared<rttb::masks::OTBMaskAccessor>(rtStructureSet->getStructure(0), doseAccessor1->getGeometricInfo());
			
			io::mask::ITKImageMaskAccessorConverter maskAccessorConverter(maskAccessorPtr);

			CHECK_NO_THROW(maskAccessorConverter.process());
			CHECK_NO_THROW(maskAccessorConverter.getITKImage());

			//2) Read itk image, generate mask and convert it back to itk image, check equal
			MaskAccessorPointer maskAccessorPtr2 = io::mask::ITKImageFileMaskAccessorGenerator(Mask_FILENAME.c_str()).generateMaskAccessor();
			io::mask::ITKImageMaskAccessorConverter maskAccessorConverter2(maskAccessorPtr2);
			maskAccessorConverter2.process();

			typedef itk::Image< DoseTypeGy, 3 >         MaskImageType;
			typedef itk::ImageFileReader<MaskImageType> ReaderType;

			ITKImageTypePointer convertedImagePtr = maskAccessorConverter2.getITKImage();

			io::mask::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer expectedImage =
			    lit::TestImageIO<unsigned char, io::mask::ITKImageMaskAccessor::ITKMaskImageType>::readImage(
			        Mask_FILENAME);

			CHECK_EQUAL(convertedImagePtr->GetOrigin()[0], expectedImage->GetOrigin()[0]);
			CHECK_EQUAL(convertedImagePtr->GetOrigin()[1], expectedImage->GetOrigin()[1]);
			CHECK_EQUAL(convertedImagePtr->GetOrigin()[2], expectedImage->GetOrigin()[2]);

			CHECK_EQUAL(convertedImagePtr->GetSpacing()[0], expectedImage->GetSpacing()[0]);
			CHECK_EQUAL(convertedImagePtr->GetSpacing()[1], expectedImage->GetSpacing()[1]);
			CHECK_EQUAL(convertedImagePtr->GetSpacing()[2], expectedImage->GetSpacing()[2]);


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

