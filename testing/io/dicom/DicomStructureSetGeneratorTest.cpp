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

#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbDicomIODStructureSetGenerator.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace testing
	{

		/*!@brief DicomIOTest - test structure set generator for dicom data
				1) test dicom file structure set generator
				2) test dicom IOD structure set generator
		*/

		int DicomStructureSetGeneratorTest(int argc, char* argv[])
		{
			typedef core::StructureSet::Pointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: structure file name


			std::string RTSTRUCT_FILENAME;


			if (argc > 1)
			{
				RTSTRUCT_FILENAME = argv[1];
			}


			/* structure set */
			//1) test dicom file structure set generator
			CHECK_NO_THROW(io::dicom::DicomFileStructureSetGenerator(""));
			CHECK_NO_THROW(io::dicom::DicomFileStructureSetGenerator("Test.test"));
			CHECK_THROW_EXPLICIT(io::dicom::DicomFileStructureSetGenerator("Test.test").generateStructureSet(),
			                     rttb::core::InvalidParameterException);
			CHECK_NO_THROW(io::dicom::DicomFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str()));

			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();


			//2) test dicom IOD structure set generator
			OFCondition status;
			DcmFileFormat fileformat;
			boost::shared_ptr<DRTStructureSetIOD> drtStrSetIOD = boost::make_shared<DRTStructureSetIOD>();

			CHECK_NO_THROW(io::dicom::DicomIODStructureSetGenerator generator1(drtStrSetIOD));
			CHECK_THROW_EXPLICIT(io::dicom::DicomIODStructureSetGenerator(drtStrSetIOD).generateStructureSet(),
			                     core::InvalidParameterException);

			fileformat.loadFile(RTSTRUCT_FILENAME.c_str());
			drtStrSetIOD->read(*fileformat.getDataset());

			CHECK_NO_THROW(io::dicom::DicomIODStructureSetGenerator generator2(drtStrSetIOD));
			CHECK_NO_THROW(io::dicom::DicomIODStructureSetGenerator(drtStrSetIOD).generateStructureSet());

			StructureSetPointer rtStructureSet2 = io::dicom::DicomIODStructureSetGenerator(
			        drtStrSetIOD).generateStructureSet();
			CHECK_EQUAL(rtStructureSet2->getNumberOfStructures(), rtStructureSet->getNumberOfStructures());



			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

