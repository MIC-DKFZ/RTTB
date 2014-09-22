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

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbVirtuosFileStructureSetGenerator.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbIndexOutOfBoundsException.h"


namespace rttb{

	namespace testing{

		/*! @brief TripStructureIOTest - test the IO for virtuos data
		1) test virtuos structure import

		Since the same class is used for the import of VIRTUOS structures, the constructors are
		tested more thoroughly in VirtuosStructureIOTest.

		WARNING: The values for comparison need to be adjusted if the input files are changed!

		@see VirtuosStructureIOTest

		@todo Test is sucessful for import in any CT (ctx) and on virtuos doses. 
		import on trip dose file will be handled by a new issue. Preliminary code can be found 
		at the end of this file.
		*/

		int TripStructureIOTest(int argc, char* argv[] )
		{
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
			typedef core::StructureSet::StructTypePointer StructTypePointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: structure file name		".../testing/data/Virtuos/NHH030000.vdx" 
			//           2: ctx  file name (virtuos)	".../testing/data/Virtuos/NHH030000.ctx.gz"
			//	       3: ctx  file name (trip)		".../testing/data/Virtuos/NHH030000.ctx"
			//		   4: dose file name (virtuos)	".../testing/data/Virtuos/NHH030101.dos.gz"
			//		   5: dose file name (trip)		".../testing/data/Virtuos/NHH030101.dos"

			/*! @todo provide an adequate number of test data. Does not work with other test data currently provided. */

			std::string RTSTRUCT_FILENAME;
			std::string CT_FILENAME;
			std::string CT_FILENAME_TRIP;
			std::string DOSE_FILENAME;
			std::string DOSE_FILENAME_TRIP;

			if (argc>1)
			{
				RTSTRUCT_FILENAME = argv[1];
			}
			if (argc>2)
			{
				CT_FILENAME = argv[2];
			}
			if (argc>3)
			{
				CT_FILENAME_TRIP = argv[3];
			}
			if (argc>4)
			{
				DOSE_FILENAME = argv[4];
			}
			if (argc>5)
			{
				DOSE_FILENAME_TRIP = argv[5];
			}

			/* structure set */
			StructureSetPointer rtStructureSet=io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),CT_FILENAME.c_str()).generateStructureSet();
			//1) test structure import
			CHECK_EQUAL(1,rtStructureSet->getNumberOfStructures());

			CHECK_EQUAL("TARGET_1",(rtStructureSet->getStructure(0))->getLabel());

			core::Structure myStructure = *(rtStructureSet->getStructure(0));
			CHECK_EQUAL(320,myStructure.getNumberOfEndpoints());

			PolygonSequenceType myPolygonSeq = myStructure.getStructureVector();
			CHECK_EQUAL(8,myPolygonSeq.size());

			PolygonType myPolygon = myPolygonSeq.at(0);

			CHECK_CLOSE(125.975,myPolygon.at(0).x(),errorConstant);
			CHECK_CLOSE(118.120,myPolygon.at(0).y(),errorConstant);
			CHECK_CLOSE(121.000,myPolygon.at(0).z(),errorConstant);

			CHECK_CLOSE(124.487,myPolygon.at(1).x(),errorConstant);
			CHECK_CLOSE(118.477,myPolygon.at(1).y(),errorConstant);
			CHECK_CLOSE(121.000,myPolygon.at(1).z(),errorConstant);

			CHECK_CLOSE(117.870,myPolygon.at(10).x(),errorConstant);
			CHECK_CLOSE(129.275,myPolygon.at(10).y(),errorConstant);
			CHECK_CLOSE(121.000,myPolygon.at(10).z(),errorConstant);

			CHECK_CLOSE(129.025,myPolygon.at(20).x(),errorConstant);
			CHECK_CLOSE(137.380,myPolygon.at(20).y(),errorConstant);
			CHECK_CLOSE(121.000,myPolygon.at(20).z(),errorConstant);

			CHECK_CLOSE(127.5,myPolygon.at(myPolygon.size()-1).x(),errorConstant);
			CHECK_CLOSE(118,myPolygon.at(myPolygon.size()-1).y(),errorConstant);
			CHECK_CLOSE(121.000,myPolygon.at(myPolygon.size()-1).z(),errorConstant);

			//The original Virtuos contours are closed (last point = 1st point), but the ones used here do not have this double point.


			/* structure set on trip */
			StructureSetPointer rtStructureSet2=io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),CT_FILENAME_TRIP.c_str()).generateStructureSet();
			//1) test structure import
			CHECK_EQUAL(1,rtStructureSet2->getNumberOfStructures());
			CHECK_EQUAL(rtStructureSet2->getNumberOfStructures(),rtStructureSet2->getNumberOfStructures());

			CHECK_EQUAL("TARGET_1",(rtStructureSet2->getStructure(0))->getLabel());

			for(int s = 0; s < rtStructureSet2->getNumberOfStructures(); s++){
				CHECK_EQUAL((rtStructureSet->getStructure(s))->getLabel(),(rtStructureSet2->getStructure(s))->getLabel());

				core::Structure myStructure2 = *(rtStructureSet2->getStructure(s));
				core::Structure myStructure1c = *(rtStructureSet->getStructure(s));
				CHECK_EQUAL(myStructure1c.getNumberOfEndpoints(),myStructure2.getNumberOfEndpoints());
				PolygonSequenceType myPolygonSeq2 = myStructure2.getStructureVector();
				PolygonSequenceType myPolygonSeq1c = myStructure2.getStructureVector();
				CHECK_EQUAL(myPolygonSeq1c.size(),myPolygonSeq2.size());

				for(int ps = 0; ps < myPolygonSeq2.size(); ps++){
					PolygonType myPolygon1c = myPolygonSeq1c.at(ps);
					PolygonType myPolygon2 = myPolygonSeq2.at(ps);
					CHECK_CLOSE(myPolygon1c.at(0).x(),myPolygon2.at(0).x(),errorConstant);
					CHECK_CLOSE(myPolygon1c.at(0).y(),myPolygon2.at(0).y(),errorConstant);
					CHECK_CLOSE(myPolygon1c.at(0).z(),myPolygon2.at(0).z(),errorConstant);
				}
			}

			/* structure set on dose */
			StructureSetPointer rtStructureSet3=io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),DOSE_FILENAME.c_str()).generateStructureSet();
			//1) test structure import, should be equal to structure imported on CT-image
			CHECK_EQUAL(1,rtStructureSet3->getNumberOfStructures());
			CHECK_EQUAL(rtStructureSet2->getNumberOfStructures(),rtStructureSet3->getNumberOfStructures());

			CHECK_EQUAL("TARGET_1",(rtStructureSet3->getStructure(0))->getLabel());

			for(int s = 0; s < rtStructureSet3->getNumberOfStructures(); s++){
				CHECK_EQUAL((rtStructureSet3->getStructure(s))->getLabel(),(rtStructureSet3->getStructure(s))->getLabel());

				core::Structure myStructure3 = *(rtStructureSet3->getStructure(s));
				core::Structure myStructure2c = *(rtStructureSet2->getStructure(s));
				CHECK_EQUAL(myStructure2c.getNumberOfEndpoints(),myStructure3.getNumberOfEndpoints());
				PolygonSequenceType myPolygonSeq3 = myStructure3.getStructureVector();
				PolygonSequenceType myPolygonSeq2c = myStructure3.getStructureVector();
				CHECK_EQUAL(myPolygonSeq2c.size(),myPolygonSeq3.size());

				for(int ps = 0; ps < myPolygonSeq3.size(); ps++){
					PolygonType myPolygon2c = myPolygonSeq2c.at(ps);
					PolygonType myPolygon3 = myPolygonSeq3.at(ps);
					CHECK_CLOSE(myPolygon2c.at(0).x(),myPolygon3.at(0).x(),errorConstant);
					CHECK_CLOSE(myPolygon2c.at(0).y(),myPolygon3.at(0).y(),errorConstant);
					CHECK_CLOSE(myPolygon2c.at(0).z(),myPolygon3.at(0).z(),errorConstant);
				}
			}

			/* structure set on trip dose*/
			/* @todo loading structure fails currently keep code and fix in seperate issue*/
			StructureSetPointer rtStructureSet4=io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),DOSE_FILENAME_TRIP.c_str()).generateStructureSet();
			//1) test structure import, should be equal to structure imported on CT-image
			CHECK_EQUAL(1,rtStructureSet4->getNumberOfStructures());
			CHECK_EQUAL(rtStructureSet2->getNumberOfStructures(),rtStructureSet4->getNumberOfStructures());

			CHECK_EQUAL("TARGET_1",(rtStructureSet4->getStructure(0))->getLabel());

			int s = 0; 

			core::Structure myStructure4 = *(rtStructureSet4->getStructure(s));
			core::Structure myStructure2c = *(rtStructureSet2->getStructure(s));
			CHECK_EQUAL(myStructure2c.getNumberOfEndpoints(),myStructure4.getNumberOfEndpoints());
			PolygonSequenceType myPolygonSeq4 = myStructure4.getStructureVector();
			PolygonSequenceType myPolygonSeq2c = myStructure2c.getStructureVector();
			CHECK_EQUAL(myPolygonSeq2c.size(),myPolygonSeq4.size());

			for(int ps = 0; ps < myPolygonSeq4.size(); ps++){
				PolygonType myPolygon2c = myPolygonSeq2c.at(ps);
				PolygonType myPolygon4 = myPolygonSeq4.at(ps);
				CHECK_CLOSE(myPolygon2c.at(0).x(),myPolygon4.at(0).x(),errorConstant);
				CHECK_CLOSE(myPolygon2c.at(0).y(),myPolygon4.at(0).y(),errorConstant);
				CHECK_CLOSE(myPolygon2c.at(0).z(),myPolygon4.at(0).z(),errorConstant);
			}

			/**/
			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

