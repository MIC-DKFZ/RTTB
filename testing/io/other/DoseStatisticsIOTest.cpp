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
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "litCheckMacros.h"

#include "rttbDoseStatistics.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbDoseStatisticsXMLWriter.h"
#include "rttbDoseStatisticsXMLReader.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbInvalidParameterException.h"
#include "rttbNullPointerException.h"

#include "../../core/DummyDoseAccessor.h"

#include "CompareDoseStatistic.h"

namespace rttb
{

	namespace testing
	{

		/*! @brief DoseStatisticsIOTest - test the DoseStatisticsIO for dose statistics
		1) test writing statistics to xml file
        2) test reading statistics from XML file and compare DoseStatistics
        3) test writing statistics to string
		*/

		int DoseStatisticsIOTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::DoseIteratorInterface::Pointer DoseIteratorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			/* generate dummy dose */
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor = boost::make_shared<DummyDoseAccessor>();
			DoseAccessorPointer spDoseAccessor(spTestDoseAccessor);

			boost::shared_ptr<core::GenericDoseIterator> spTestDoseIterator =
			    boost::make_shared<core::GenericDoseIterator>(spDoseAccessor);
			DoseIteratorPointer spDoseIterator(spTestDoseIterator);

			rttb::algorithms::DoseStatisticsCalculator myDoseStatsCalculator(spDoseIterator);
			auto myDoseStatsSimple = myDoseStatsCalculator.calculateDoseStatistics();
			auto myDoseStatsComplex = myDoseStatsCalculator.calculateDoseStatistics(true);

			auto doseStatisticsXMLWriter = io::other::DoseStatisticsXMLWriter();

            //1) test writing statistics to xml file
            CHECK_THROW_EXPLICIT(doseStatisticsXMLWriter.writeDoseStatistics(nullptr, "aFilename.txt"), core::NullPointerException);

			FileNameString filenameSimple = "testStatisticsSimple.xml";
			CHECK_NO_THROW(doseStatisticsXMLWriter.writeDoseStatistics(myDoseStatsSimple, filenameSimple));
            CHECK(boost::filesystem::exists(filenameSimple));

            FileNameString filenameComplex = "testStatisticsComplex.xml";
            CHECK_NO_THROW(doseStatisticsXMLWriter.writeDoseStatistics(myDoseStatsComplex, filenameComplex));
            CHECK(boost::filesystem::exists(filenameComplex));

            //2) test reading statistics from XML file and compare DoseStatistics
			io::other::DoseStatisticsXMLReader readerSimple= io::other::DoseStatisticsXMLReader(filenameSimple);
			boost::shared_ptr<rttb::algorithms::DoseStatistics> rereadSimplyDose;
			CHECK_NO_THROW(rereadSimplyDose = readerSimple.generateDoseStatistic());
			CHECK(checkEqualDoseStatistic(myDoseStatsSimple, rereadSimplyDose));

			io::other::DoseStatisticsXMLReader readerComplex = io::other::DoseStatisticsXMLReader(filenameComplex);
			boost::shared_ptr<rttb::algorithms::DoseStatistics> rereadComplexDose;
			
			CHECK_NO_THROW(rereadComplexDose = readerComplex.generateDoseStatistic());
			CHECK(checkEqualDoseStatistic(myDoseStatsComplex, rereadComplexDose));

            io::other::DoseStatisticsXMLReader readerInvalidFilename = io::other::DoseStatisticsXMLReader("invalidFilename.xml");
            CHECK_THROW_EXPLICIT(readerInvalidFilename.generateDoseStatistic(), core::InvalidParameterException);

            //delete files again
            CHECK_EQUAL(std::remove(filenameSimple.c_str()), 0);
            CHECK_EQUAL(std::remove(filenameComplex.c_str()),0);

            //3) test writing statistics to string
			boost::property_tree::ptree ptSimple = doseStatisticsXMLWriter.writeDoseStatistics(myDoseStatsSimple);
			XMLString strSimple = doseStatisticsXMLWriter.writerDoseStatisticsToString(myDoseStatsSimple);

            CHECK_THROW_EXPLICIT(doseStatisticsXMLWriter.writerDoseStatisticsToString(nullptr), core::NullPointerException);

			boost::property_tree::ptree ptComplex = doseStatisticsXMLWriter.writeDoseStatistics(myDoseStatsComplex);
			XMLString strComplex = doseStatisticsXMLWriter.writerDoseStatisticsToString(myDoseStatsComplex);

			std::stringstream sstrSimple;
			boost::property_tree::xml_parser::write_xml(sstrSimple, ptSimple,
			        boost::property_tree::xml_writer_make_settings<std::string>('\t',
			                1));
			CHECK_EQUAL(strSimple, sstrSimple.str());

			std::stringstream sstrComplex;
			boost::property_tree::xml_parser::write_xml(sstrComplex, ptComplex,
			        boost::property_tree::xml_writer_make_settings<std::string>('\t',
			                1));
			CHECK_EQUAL(strComplex, sstrComplex.str());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb



