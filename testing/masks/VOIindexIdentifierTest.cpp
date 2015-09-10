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
// @version $Revision:
// @date $Date:
// @author $Author:
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbInvalidParameterException.h"
#include "rttbVOIindexIdentifier.h"

namespace rttb
{

	namespace testing
	{

		int VOIindexIdentifierTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			//ARGUMENTS: 1: structure file name
			std::string RTSTRUCT_FILENAME;


			if (argc > 1)
			{
				RTSTRUCT_FILENAME = argv[1];
			}

			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();

			StructureSetPointer emptyPointer = StructureSetPointer();

			/* getIndexByVoiName */
			CHECK_NO_THROW(::rttb::masks::VOIindexIdentifier testVOIindexId = ::rttb::masks::VOIindexIdentifier());
			::rttb::masks::VOIindexIdentifier testVOIindexId = ::rttb::masks::VOIindexIdentifier();
			CHECK_THROW_EXPLICIT(testVOIindexId.getIndexByVoiName(emptyPointer, "Leber"),
			                     ::rttb::core::Exception);

			CHECK_NO_THROW(testVOIindexId.getIndexByVoiName(rtStructureSet, "Leber"));
			int intVoi = 5;
			CHECK_EQUAL(testVOIindexId.getIndexByVoiName(rtStructureSet, "Leber"), intVoi);

			CHECK_NO_THROW(testVOIindexId.getIndexByVoiName(rtStructureSet, "Leber||Leb"));

			CHECK_EQUAL(testVOIindexId.getIndexByVoiName(rtStructureSet, "Leber"), intVoi);
			CHECK_EQUAL(testVOIindexId.getIndexByVoiName(rtStructureSet, "Leber||Leb"), intVoi);

			CHECK_THROW_EXPLICIT(testVOIindexId.getIndexByVoiName(rtStructureSet, "Herz"), ::rttb::core::Exception);

			/* getVoiNameByIndex */
			CHECK_THROW_EXPLICIT(testVOIindexId.getVoiNameByIndex(emptyPointer, 5),
			                     ::rttb::core::Exception);
			CHECK_EQUAL(testVOIindexId.getVoiNameByIndex(rtStructureSet, 5), "Leber");
			std::string voiName = "Herz";
			CHECK_THROW_EXPLICIT(testVOIindexId.getVoiNameByIndex(rtStructureSet, 20), ::rttb::core::Exception);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb
