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

#include "itkImageFileReader.h"

#include "rttbITKImageAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace testing
	{

		/*!@brief ITKDoseAccessorGeneratorTest - test the generators for dicom data
		1) test itk file generator generateDoseAccessor()
		2) test itk generator generateDoseAccessor()
		*/

		int ITKDoseAccessorGeneratorTest(int argc, char* argv[])
		{

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			// 1: mhd/raw file name with short image type
      // 2: mhd/raw file name with double image type

			std::string ITKSHORTFILE_FILENAME, ITKDOUBLEFILE_FILENAME, ITK2DVECTORFILE_FILENAME, ITK2DFILE_FILENAME;

			if (argc > 4)
			{
        ITKSHORTFILE_FILENAME = argv[1];
        ITKDOUBLEFILE_FILENAME = argv[2];
        ITK2DVECTORFILE_FILENAME = argv[3];
        ITK2DFILE_FILENAME = argv[4];
			}


			/* test ITKFileDoseAccessorGenerator generateDoseAccessor()*/
			CHECK_THROW_EXPLICIT(io::itk::ITKImageFileAccessorGenerator("test.test").generateDoseAccessor(),
			                     core::InvalidDoseException);
			CHECK_NO_THROW(io::itk::ITKImageFileAccessorGenerator(
        ITKSHORTFILE_FILENAME.c_str()).generateDoseAccessor());
      CHECK_NO_THROW(io::itk::ITKImageFileAccessorGenerator(
        ITKDOUBLEFILE_FILENAME.c_str()).generateDoseAccessor());
      CHECK_THROW_EXPLICIT(io::itk::ITKImageFileAccessorGenerator(
        ITK2DFILE_FILENAME.c_str()).generateDoseAccessor(), core::InvalidParameterException);
      CHECK_THROW(io::itk::ITKImageFileAccessorGenerator(
        ITK2DVECTORFILE_FILENAME.c_str()).generateDoseAccessor());

			/* test ITKDoseAccessorGenerator generateDoseAccessor()*/
			typedef itk::Image< DoseTypeGy, 3 >         DoseImageType;
			typedef itk::ImageFileReader<DoseImageType> ReaderType;

			DoseImageType::Pointer invalidDose = DoseImageType::New();

			ReaderType::Pointer reader = ReaderType::New();

			CHECK_THROW_EXPLICIT(io::itk::ITKImageAccessorGenerator(
			                         invalidDose.GetPointer()).generateDoseAccessor(), core::InvalidDoseException);

			reader->SetFileName(ITKSHORTFILE_FILENAME);
			//important to update the reader (won't work without)
			reader->Update();

			CHECK_NO_THROW(io::itk::ITKImageAccessorGenerator(reader->GetOutput()).generateDoseAccessor());


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

