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

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDoseStatistics.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbDoseStatisticsXMLWriter.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbVirtuosFileStructureSetGenerator.h"
#include "rttbBoostMaskAccessor.h"


namespace rttb
{

	namespace testing
	{

		/*! @brief OtherIOTest - test the IO for dose statistics
		    1) test exception
			2) test writing statistics to xml file
		*/

		int DoseStatisticsIOVirtuosTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
			typedef boost::shared_ptr<rttb::algorithms::DoseStatistics> DoseStatisticsPtr;
			typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;
			typedef boost::shared_ptr<rttb::core::GeometricInfo> GeometricInfoPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			std::string Struct_FILENAME;
			std::string BPLCT_FILENAME;
			std::string Dose_FILENAME;
			std::string Plan_FILENAME;
			std::string Struct_NAME;

			if (argc > 5)
			{
				Struct_FILENAME = argv[1];
				BPLCT_FILENAME = argv[2];
				Dose_FILENAME = argv[3];
				Plan_FILENAME = argv[4];
				Struct_NAME = argv[5];
			}
			else
			{
				std::cout << "at least 5 arguments are expected in DoseStatisticsIOVirtuosTest." << std::endl;
			}

			/* generate dummy dose */
			io::virtuos::VirtuosPlanFileDoseAccessorGenerator doseAccessorGenerator(Dose_FILENAME.c_str(), Plan_FILENAME.c_str());

			DoseAccessorPointer doseAccessor(doseAccessorGenerator.generateDoseAccessor());

			StructureSetPointer rtStructureSet = io::virtuos::VirtuosFileStructureSetGenerator(
			        Struct_FILENAME.c_str(), BPLCT_FILENAME.c_str()).generateStructureSet();

			GeometricInfoPointer geometricInfoPtr = boost::make_shared<rttb::core::GeometricInfo>(doseAccessor->getGeometricInfo());
			MaskAccessorPointer maskAccessorPtr = boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
			                                      (rtStructureSet->getStructure(3), doseAccessor->getGeometricInfo());
			maskAccessorPtr->updateMask();

			boost::shared_ptr<core::GenericMaskedDoseIterator> spTestDoseIterator =
			    boost::make_shared<core::GenericMaskedDoseIterator>(maskAccessorPtr, doseAccessor);
			DoseIteratorPointer spDoseIterator(spTestDoseIterator);

			rttb::algorithms::DoseStatisticsCalculator myDoseStatsCalculator(spDoseIterator);
			rttb::algorithms::DoseStatistics::DoseStatisticsPointer doseStatistics =
			    myDoseStatsCalculator.calculateDoseStatistics(true);

			/* test writing statistcs to xml file */
			FileNameString fN = "doseStatisticsVirtuosComplex.xml";
			CHECK_NO_THROW(io::other::writeDoseStatistics(doseStatistics, fN));

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

