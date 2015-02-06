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

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

