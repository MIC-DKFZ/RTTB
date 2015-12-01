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

			CHECK_NO_THROW(::rttb::masks::VOIindexIdentifier testVOIindexId
			               = ::rttb::masks::VOIindexIdentifier());
			::rttb::masks::VOIindexIdentifier testVOIindexId = ::rttb::masks::VOIindexIdentifier();

			/* getIndexByVoiName */
			CHECK_THROW_EXPLICIT(testVOIindexId.getIndexByVoiName(emptyPointer, "Leber"),
			                     ::rttb::core::Exception);
			CHECK_THROW_EXPLICIT(testVOIindexId.getIndexByVoiName(rtStructureSet, "Invalid"),
			                     ::rttb::core::Exception);

			unsigned int indexActual;
			unsigned int indexExpected = 5;

			CHECK_NO_THROW(indexActual = testVOIindexId.getIndexByVoiName(rtStructureSet, "Leber"));
			CHECK_EQUAL(indexActual, indexExpected);


			/*getIndicesByVoiRegex*/
			CHECK_THROW_EXPLICIT(testVOIindexId.getIndicesByVoiRegex(emptyPointer, "Leber"),
			                     ::rttb::core::Exception);

			std::vector<unsigned int> vectorActual;
			std::vector<unsigned int> vectorExpected;
			vectorExpected.push_back(5);
			CHECK_NO_THROW(vectorActual = testVOIindexId.getIndicesByVoiRegex(rtStructureSet, "Leber"));
			CHECK_EQUAL(vectorActual.size(), vectorExpected.size());
			CHECK_EQUAL(vectorActual.at(0), vectorExpected.at(0));

			vectorExpected.clear();
			vectorExpected.push_back(2);
			vectorExpected.push_back(3);
			CHECK_NO_THROW(vectorActual = testVOIindexId.getIndicesByVoiRegex(rtStructureSet, "Niere.*"));

			CHECK_EQUAL(vectorActual.size(), vectorExpected.size());
			CHECK_EQUAL(vectorActual.at(0), vectorExpected.at(0));
			CHECK_EQUAL(vectorActual.at(1), vectorExpected.at(1));

			CHECK_NO_THROW(vectorActual = testVOIindexId.getIndicesByVoiRegex(rtStructureSet, ".*"));
			CHECK_EQUAL(vectorActual.size(), 10);

			for (unsigned int index = 0; index < vectorActual.size(); index++)
			{
				CHECK_EQUAL(vectorActual.at(index), index);
			}

			/* getVoiNameByIndex */
			CHECK_THROW_EXPLICIT(testVOIindexId.getVoiNameByIndex(emptyPointer, 5),
			                     ::rttb::core::Exception);
			CHECK_THROW_EXPLICIT(testVOIindexId.getVoiNameByIndex(rtStructureSet, 20), ::rttb::core::Exception);

			CHECK_EQUAL(testVOIindexId.getVoiNameByIndex(rtStructureSet, 5), "Leber");


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

