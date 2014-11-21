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
// @version $Revision: 841 $ (last changed revision)
// @date $Date: 2014-11-07 14:39:21 +0100 (Fr, 07 Nov 2014) $ (last change date)
// @author $Author: zhangl $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <iomanip>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDicomFileReaderHelper.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief DicomDoseAccessorGeneratorTest - test the generators for dicom data
		1) test getFileNamesWithSameUID() with a directory name
		2) test getFileNames() with a RTDOSE file name and check equal with getFileNamesWithSameUID()
		3) test getFileNames() with a RTSTRUCT file name
		*/

		int DicomFileReaderHelperTest(int argc, char* argv[])
		{
			typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPtr;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			//           1: helax directory name
			//           2: dose file name
			//           3: structure file name

			std::string RT_DIRNAME;
			std::string RTDOSE_FILENAME;
			std::string RTStr_FILENAME;

			if (argc > 3)
			{
				RT_DIRNAME = argv[1];
				RTDOSE_FILENAME = argv[2];
				RTStr_FILENAME = argv[3];
			}
			rttb::io::dicom::Modality doseModality= {rttb::io::dicom::Modality::RTDOSE};
			rttb::io::dicom::Modality strModality= {rttb::io::dicom::Modality::RTSTRUCT};

			//1) test getFileNamesWithSameUID() with a directory name
			std::vector<FileNameType> fileVector = rttb::io::dicom::getFileNamesWithSameUID(RT_DIRNAME, doseModality);
			CHECK_EQUAL(fileVector.size(), 52);

			//2) test getFileNames() with a RTDOSE file name and check equal with getFileNamesWithSameUID()
			std::vector<FileNameType> fileVector2 = rttb::io::dicom::getFileNames(RTDOSE_FILENAME);
			for(int i=0; i<fileVector.size(); i++){
				CHECK_EQUAL(fileVector.at(i), fileVector2.at(i));
			}
			
			//3) test getFileNames() with a RTSTRUCT file name
			fileVector2 = rttb::io::dicom::getFileNames(RTStr_FILENAME);
			CHECK_EQUAL(fileVector2.size(), 1);

			fileVector = rttb::io::dicom::getFileNamesWithSameUID(RT_DIRNAME, strModality);
			CHECK_EQUAL(fileVector.size(), 1);
			CHECK_EQUAL(fileVector.at(0), fileVector2.at(0));

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

