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
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author $Author$ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <iomanip>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomIODDoseAccessorGenerator.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief DicomDoseAccessorGeneratorTest - test the generators for dicom data
		1) test dicom file generator generateDoseAccessor()
		2) test dicom IOD generator generateDoseAccessor()
		*/

		int DicomDoseAccessorGeneratorTest(int argc, char* argv[])
		{
			typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPtr;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			//           1: dose1 file name
			//           2: dose2 file name
			//           3: dose3 file name

			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;
			std::string RTDOSE3_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE2_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTDOSE3_FILENAME = argv[3];
			}



			OFCondition status;
			DcmFileFormat fileformat;

			/* test DicomFileDoseAccessorGenerator generateDoseAccessor()*/
			CHECK_THROW_EXPLICIT(io::dicom::DicomFileDoseAccessorGenerator("test.test").generateDoseAccessor(),
			                     core::InvalidDoseException);
			CHECK_NO_THROW(io::dicom::DicomFileDoseAccessorGenerator(
			                   RTDOSE_FILENAME.c_str()).generateDoseAccessor());
			CHECK_NO_THROW(io::dicom::DicomFileDoseAccessorGenerator(
			                   RTDOSE2_FILENAME.c_str()).generateDoseAccessor());
			CHECK_NO_THROW(io::dicom::DicomFileDoseAccessorGenerator(
			                   RTDOSE3_FILENAME.c_str()).generateDoseAccessor());

			/* test DicomIODDoseAccessorGenerator generateDoseAccessor()*/

			DRTDoseIODPtr dose = boost::make_shared<DRTDoseIOD>();

			CHECK_THROW_EXPLICIT(io::dicom::DicomIODDoseAccessorGenerator(dose).generateDoseAccessor(),
			                     io::dicom::DcmrtException);

			fileformat.loadFile(RTDOSE_FILENAME.c_str());

			dose->read(*fileformat.getDataset());

			CHECK_NO_THROW(io::dicom::DicomIODDoseAccessorGenerator(dose).generateDoseAccessor());


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

