// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDVH.h"
#include "rttbDVHSet.h"
#include "rttbDVHXMLFileReader.h"
#include "rttbDVHXMLFileWriter.h"
#include "rttbInvalidParameterException.h"
#include "rttbNullPointerException.h"

#include "../../core/DummyDVHGenerator.h"

#include "CompareDVH.h"

namespace rttb
{

	namespace testing
	{

		/*! @brief DVHXMLIOTest - test the IO for DVH xml data
		1) test writing dvh to xml file
		2) test reading DVH from xml file
		*/

		int DVHXMLIOTest(int argc, char* argv[])
		{
			typedef core::DVH::DVHPointer DVHPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			/* generate dummy DVH */
			const IDType structureIDPrefix = "myStructure";
			const IDType doseID = "myDose";

			DummyDVHGenerator dvhGenerator;

			DVHPointer spMyDVH = boost::make_shared<core::DVH>(dvhGenerator.generateDVH(structureIDPrefix,
			                     doseID));

			// 1) test writing DVH to xml file
			DVHType typeCum = {DVHType::Cumulative};
			DVHType typeDiff = {DVHType::Differential};
			FileNameString fN1 = "test.xml";
			CHECK_NO_THROW(io::other::DVHXMLFileWriter(fN1, typeDiff));
			CHECK_NO_THROW(io::other::DVHXMLFileWriter(fN1, typeCum));

			io::other::DVHXMLFileWriter dvhWriter(fN1, typeCum);

			CHECK_EQUAL(fN1, dvhWriter.getFileName());

			FileNameString fN2 = "otherFile.xml";
			CHECK_NO_THROW(dvhWriter.setFileName(fN2));
			CHECK_EQUAL(fN2, dvhWriter.getFileName());

			CHECK_EQUAL(DVHType::Cumulative, dvhWriter.getDVHType().Type);
			CHECK_NO_THROW(dvhWriter.setDVHType(typeDiff));
			CHECK_EQUAL(DVHType::Differential, dvhWriter.getDVHType().Type);

			DVHPointer emptyDvh;
			CHECK_THROW_EXPLICIT(dvhWriter.writeDVH(emptyDvh), core::NullPointerException);
			//CHECK_THROW_EXPLICIT(dvhWriter.writeDVH(spMyDVH), core::InvalidParameterException);
			CHECK_NO_THROW(dvhWriter.setDVHType(typeDiff));
			CHECK_NO_THROW(dvhWriter.writeDVH(spMyDVH));

			// 2) test reading DVH from xml file
			io::other::DVHXMLFileReader dvhReader(fN2);

			DVHPointer importedDVH = dvhReader.generateDVH();

			CHECK_EQUAL(*importedDVH, *spMyDVH);


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

