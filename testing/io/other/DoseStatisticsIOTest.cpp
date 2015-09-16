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
#include <boost/lexical_cast.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDoseStatistics.h"
#include "rttbDoseStatisticsXMLWriter.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbInvalidParameterException.h"
#include "rttbNullPointerException.h"

#include "../../core/DummyDoseAccessor.h"


namespace rttb
{

	namespace testing
	{

		/*! @brief OtherIOTest - test the IO for dose statistics
		    1) test exception
			2) test writing statistcs to xml file
		*/

		int DoseStatisticsIOTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
			typedef boost::shared_ptr<rttb::algorithms::DoseStatistics> DoseStatisticsPtr;

			PREPARE_DEFAULT_TEST_REPORTING;

			/* generate dummy dose */
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor = boost::make_shared<DummyDoseAccessor>();
			DoseAccessorPointer spDoseAccessor(spTestDoseAccessor);

			boost::shared_ptr<core::GenericDoseIterator> spTestDoseIterator =
			    boost::make_shared<core::GenericDoseIterator>(spDoseAccessor);
			DoseIteratorPointer spDoseIterator(spTestDoseIterator);

			rttb::algorithms::DoseStatistics myDoseStats(spDoseIterator);
			DoseStatisticsPtr myDoseStatsPtr = boost::make_shared<rttb::algorithms::DoseStatistics>
			                                   (myDoseStats);

			/* test exception */
			CHECK_THROW_EXPLICIT(io::other::writeDoseStatistics(myDoseStatsPtr, "test.test", 0),
			                     core::InvalidParameterException);


			/* test writing statistcs to xml file */
			FileNameString fN = "testStatistics.xml";
			CHECK_NO_THROW(io::other::writeDoseStatistics(myDoseStatsPtr, fN));

			/* test writing statistcs to string */
			boost::property_tree::ptree pt = io::other::writeDoseStatistics(myDoseStatsPtr);
			XMLString str = io::other::writerDoseStatisticsToString(myDoseStatsPtr);

			std::stringstream sstr;
			boost::property_tree::xml_parser::write_xml(sstr, pt);
			CHECK_EQUAL(str, sstr.str());




			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

