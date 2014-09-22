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

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "rttbBaseType.h"
#include "rttbITKImageDoseAccessor.h"
#include "rttbITKImageFileDoseAccessorGenerator.h"
#include "rttbITKImageDoseAccessorGenerator.h"
#include "rttbInvalidDoseException.h"


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
			//           1: mhd/raw file name

			std::string RTDOSE_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}


			/* test ITKFileDoseAccessorGenerator generateDoseAccessor()*/
			CHECK_THROW_EXPLICIT(io::itk::ITKImageFileDoseAccessorGenerator("test.test").generateDoseAccessor(),
			                     core::InvalidDoseException);
			CHECK_NO_THROW(io::itk::ITKImageFileDoseAccessorGenerator(
			                   RTDOSE_FILENAME.c_str()).generateDoseAccessor());

			/* test ITKDoseAccessorGenerator generateDoseAccessor()*/
			typedef itk::Image< DoseTypeGy, 3 >         DoseImageType;
			typedef itk::ImageFileReader<DoseImageType> ReaderType;

			DoseImageType::ConstPointer invalidDose = DoseImageType::New();

			ReaderType::Pointer reader = ReaderType::New();

			CHECK_THROW_EXPLICIT(io::itk::ITKImageDoseAccessorGenerator(
			                         invalidDose.GetPointer()).generateDoseAccessor(), core::InvalidDoseException);

			reader->SetFileName(RTDOSE_FILENAME);
			//important to update the reader (won't work without)
			reader->Update();

			CHECK_NO_THROW(io::itk::ITKImageDoseAccessorGenerator(reader->GetOutput()).generateDoseAccessor());


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

