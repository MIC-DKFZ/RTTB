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

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "rttbBaseType.h"
#include "rttbInvalidDoseException.h"
#include "rttbITKImageFileMaskAccessorGenerator.h"
#include "rttbITKImageMaskAccessorGenerator.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief MaskAccessorGeneratorTest - test the generators for dicom data
		1) test itk file generator generateDoseAccessor()
		2) test itk generator generateDoseAccessor()
		*/

		int ITKMaskAccessorGeneratorTest(int argc, char* argv[])
		{

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			//           1: mhd/raw file name

			std::string Mask_FILENAME;

			if (argc > 1)
			{
				Mask_FILENAME = argv[1];
			}

			
			/* test ITKImageFileMaskAccessorGenerator generateDoseAccessor()*/
			CHECK_THROW_EXPLICIT(io::itk::ITKImageFileMaskAccessorGenerator("test.test").generateMaskAccessor(),
			                     core::InvalidDoseException);
			CHECK_NO_THROW(io::itk::ITKImageFileMaskAccessorGenerator(
			                   Mask_FILENAME.c_str()).generateMaskAccessor());

			/* test ITKImageMaskAccessorGenerator generateDoseAccessor()*/
			typedef itk::Image< DoseTypeGy, 3 >         MaskImageType;
			typedef itk::ImageFileReader<MaskImageType> ReaderType;

			MaskImageType::ConstPointer invalidDose = MaskImageType::New();

			ReaderType::Pointer reader = ReaderType::New();

			CHECK_THROW_EXPLICIT(io::itk::ITKImageMaskAccessorGenerator(
			                         invalidDose.GetPointer()).generateMaskAccessor(), core::InvalidDoseException);

			reader->SetFileName(Mask_FILENAME);
			//important to update the reader (won't work without)
			reader->Update();

			CHECK_NO_THROW(io::itk::ITKImageMaskAccessorGenerator(reader->GetOutput()).generateMaskAccessor());
			
			io::itk::ITKImageMaskAccessorGenerator::MaskAccessorPointer maskAcc = io::itk::ITKImageMaskAccessorGenerator(reader->GetOutput()).generateMaskAccessor();

			CHECK_NO_THROW( maskAcc->getRelevantVoxelVector());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

