// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// (c) Copyright 2007, DKFZ, Heidelberg, Germany
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR
// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author zhangl (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/trunk/testing/core/DVHCalculatorTest.cpp $
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
#include "rttbDVHTxtFileReader.h"
#include "rttbDVHTxtFileWriter.h"
#include "rttbDVHXMLFileReader.h"
#include "rttbDVHXMLFileWriter.h"
#include "rttbInvalidParameterException.h"
#include "rttbNullPointerException.h"

#include "../../core/DummyDVHGenerator.h"


namespace rttb{

	namespace testing{

		/*! @brief DVHXMLIOTest - test the IO for DVH xml data
		1) test writing dvh to xml file
		2) test reading DVH from xml file 
		3) test reading dvh from txt file and writing to xml, check equal
		*/

		int DVHXMLIOTest(int argc, char* argv[] )
		{
			typedef core::DVHSet::DVHSetType DVHSetType;
			typedef core::DVH::DVHPointer DVHPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			std::string DVHTXT_FILENAME;

			if (argc>1)
			{
				DVHTXT_FILENAME = argv[1];
			}

			/* generate dummy DVH */
			const IDType structureIDPrefix = "myStructure";
			const IDType doseID = "myDose";

			DummyDVHGenerator dvhGenerator;	  

			DVHPointer spMyDVH = boost::make_shared<core::DVH>(dvhGenerator.generateDVH(structureIDPrefix,doseID));		

			// 1) test writing DVH to xml file
			DVHType typeCum = {DVHType::Cumulative};
			DVHType typeDiff = {DVHType::Differential};
			FileNameString fN1 = "test.xml";
			CHECK_NO_THROW(io::other::DVHXMLFileWriter(fN1, typeDiff));
			CHECK_NO_THROW(io::other::DVHXMLFileWriter(fN1, typeCum));

			io::other::DVHXMLFileWriter dvhWriter (fN1, typeCum);

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

			//3) test reading dvh from txt file and writing to xml 
			io::other::DVHTxtFileReader dvhReader2(DVHTXT_FILENAME);
			DVHPointer importedDVH2 = dvhReader2.generateDVH();
			FileNameString toWrite = "test.xml";
			io::other::DVHXMLFileWriter xmlWriter(toWrite, typeDiff);
			xmlWriter.writeDVH(importedDVH2);

			io::other::DVHXMLFileReader xmlReader(toWrite);
			DVHPointer readDVH=xmlReader.generateDVH();
			const double errorConstant = 1e-7;

			CHECK_CLOSE(importedDVH2->getDeltaD(), readDVH->getDeltaD(), errorConstant);
			CHECK_CLOSE(importedDVH2->getDeltaV(), readDVH->getDeltaV(), errorConstant);
			CHECK(importedDVH2->getDoseID() == readDVH->getDoseID());
			CHECK(importedDVH2->getStructureID() == readDVH->getStructureID());
			CHECK_CLOSE(importedDVH2->getMaximum(), readDVH->getMaximum(), errorConstant);
			CHECK_CLOSE(importedDVH2->getMinimum(), readDVH->getMinimum(), errorConstant);
			CHECK_CLOSE(importedDVH2->getMean(), readDVH->getMean(), errorConstant);
			CHECK(importedDVH2->getDataDifferential().size() == readDVH->getDataDifferential().size());
			for(int i=0; i<importedDVH2->getDataDifferential().size(); i++){
				CHECK_CLOSE(importedDVH2->getDataDifferential().at(i), readDVH->getDataDifferential().at(i), errorConstant);
			}


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

