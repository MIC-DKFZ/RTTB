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

#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxIODVecDoseAccessorGenerator.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace testing
	{

		/*!@brief DicomHelaxDoseAccessorGeneratorTest - test the IO for dicom helax data
		1) test dicom helax file generator generateDoseAccessor()
		2) test dicom helax IOD vector generator generateDoseAccessor()
		*/

		int DicomHelaxDoseAccessorGeneratorTest(int argc, char* argv[])
		{
			typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPtr;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: dose directory name
			//           2: dose1 file name
			//           3: dose2 file name
			//           4: dose3 file name

			std::string RTDOSE_DIRNAME;
			std::string RTDOSE1_FILENAME;
			std::string RTDOSE2_FILENAME;
			std::string RTDOSE3_FILENAME;


			if (argc > 1)
			{
				RTDOSE_DIRNAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE1_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTDOSE2_FILENAME = argv[3];
			}

			if (argc > 4)
			{
				RTDOSE3_FILENAME = argv[4];
			}


			OFCondition status;
			DcmFileFormat fileformat;

			/* test dicom helax file generator generateDoseAccessor() */
			CHECK_THROW_EXPLICIT(
			    io::helax::DicomHelaxFileDoseAccessorGenerator("/testAsInvalidDirectoryName/").generateDoseAccessor(),
			    core::InvalidParameterException);
			CHECK_NO_THROW(io::helax::DicomHelaxFileDoseAccessorGenerator(
			                   RTDOSE_DIRNAME.c_str()).generateDoseAccessor());


			/* test dicom helax IOD vector generator generateDoseAccessor()*/
			DRTDoseIODPtr dose1 = boost::make_shared<DRTDoseIOD>();
			DRTDoseIODPtr dose2 = boost::make_shared<DRTDoseIOD>();
			DRTDoseIODPtr dose3 = boost::make_shared<DRTDoseIOD>();
			std::vector<DRTDoseIODPtr> doseVector;

			//test empty vector
			CHECK_THROW_EXPLICIT(io::helax::DicomHelaxIODVecDoseAccessorGenerator(
			                         doseVector).generateDoseAccessor(), core::InvalidParameterException);

			doseVector.push_back(dose1);
			doseVector.push_back(dose2);
			doseVector.push_back(dose3);

			//test vector with all empty dose IODs
			CHECK_THROW_EXPLICIT(io::helax::DicomHelaxIODVecDoseAccessorGenerator(
			                         doseVector).generateDoseAccessor(), core::InvalidDoseException);


			fileformat.loadFile(RTDOSE1_FILENAME.c_str());
			dose1->read(*fileformat.getDataset());
			fileformat.loadFile(RTDOSE2_FILENAME.c_str());
			dose2->read(*fileformat.getDataset());
			//test vector with one empty dose IOD
			CHECK_THROW_EXPLICIT(io::helax::DicomHelaxIODVecDoseAccessorGenerator(
			                         doseVector).generateDoseAccessor(), core::InvalidDoseException);


			fileformat.loadFile(RTDOSE3_FILENAME.c_str());
			dose3->read(*fileformat.getDataset());
			CHECK_NO_THROW(io::helax::DicomHelaxIODVecDoseAccessorGenerator(doseVector).generateDoseAccessor());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

