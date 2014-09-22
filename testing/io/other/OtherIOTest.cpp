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
#include "rttbInvalidParameterException.h"
#include "rttbNullPointerException.h"

#include "../../core/DummyDVHGenerator.h"


namespace rttb{

	namespace testing{

		/*! @brief OtherIOTest - test the IO for DVH txt data
			1) test writing dvh to text file
			2) test reading DVH from text file 
		*/

		int OtherIOTest(int argc, char* argv[] )
		{
			typedef core::DVHSet::DVHSetType DVHSetType;
			typedef core::DVH::DVHPointer DVHPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			/* generate dummy DVH */
			const IDType structureIDPrefix = "myStructure";
			const IDType doseID = "myDose";

			DummyDVHGenerator dvhGenerator;	  

			DVHPointer spMyDVH = boost::make_shared<core::DVH>(dvhGenerator.generateDVH(structureIDPrefix,doseID));		

			// 1) test writing DVH to text file
			DVHType typeCum = {DVHType::Cumulative};
			DVHType typeDiff = {DVHType::Differential};
			FileNameString fN1 = "test.txt";
			CHECK_NO_THROW(io::other::DVHTxtFileWriter(fN1, typeDiff));
			CHECK_NO_THROW(io::other::DVHTxtFileWriter(fN1, typeCum));


			io::other::DVHTxtFileWriter dvhWriter (fN1, typeCum);

			CHECK_EQUAL(fN1, dvhWriter.getFileName());

			FileNameString fN2 = "otherFile.txt";
			CHECK_NO_THROW(dvhWriter.setFileName(fN2));
			CHECK_EQUAL(fN2, dvhWriter.getFileName());

			CHECK_EQUAL(DVHType::Cumulative, dvhWriter.getDVHType().Type);
			CHECK_NO_THROW(dvhWriter.setDVHType(typeDiff));
			CHECK_EQUAL(DVHType::Differential, dvhWriter.getDVHType().Type);

			DVHPointer emptyDvh;
			CHECK_THROW_EXPLICIT(dvhWriter.writeDVH(emptyDvh),core::NullPointerException);
			CHECK_NO_THROW(dvhWriter.setDVHType(typeDiff));
			CHECK_NO_THROW(dvhWriter.writeDVH(spMyDVH));

			// 2) test reading DVH from text file 
			CHECK_NO_THROW(io::other::DVHTxtFileReader dvhReader(fN1));
			io::other::DVHTxtFileReader dvhReaderTest(fN1);
			CHECK_THROW_EXPLICIT(dvhReaderTest.generateDVH(),core::InvalidParameterException);
			CHECK_NO_THROW(io::other::DVHTxtFileReader dvhReader(fN2));

			io::other::DVHTxtFileReader dvhReader(fN2);

			CHECK_NO_THROW(dvhReader.resetFileName(fN1));
			CHECK_THROW_EXPLICIT(dvhReader.generateDVH(), core::InvalidParameterException);
			CHECK_NO_THROW(dvhReader.resetFileName(fN2));

			CHECK_NO_THROW(dvhReader.generateDVH());
			
			DVHPointer importedDVH = dvhReader.generateDVH();

			CHECK_EQUAL(*importedDVH, *spMyDVH);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

