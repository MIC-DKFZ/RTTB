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
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbVirtuosFileStructureSetGenerator.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbIndexOutOfBoundsException.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief VirtuosIOTest - test the IO for virtuos data
				1) test virtuos structure import

			WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int VirtuosStructureIOTest(int argc, char* argv[])
		{
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: structure file name	   ".../testing/data/Virtuos/NHH030000.vdx"
			//           2: ctx  file name (virtuos) ".../testing/data/Virtuos/NHH030000.ctx.gz"
			//	       3: structure file name	   ".../testing/data/Virtuos/prostate_ac/prostate_ac000.vdx"
			//           4: ctx  file name (virtuos) ".../testing/data/Virtuos/prostate_ac/prostate_ac000.ctx.gz"
			//           5: dos  file name (virtuos) ".../testing/data/ Virtuos/prostate_ac/prostate_ac101.dos.gz"

			/*! @todo provide a adequate number of test data. Does not work with other test data currently provided. Structures are not loaded! */

			std::string RTSTRUCT_FILENAME;
			std::string RTSTRUCT_FILENAME2;
			std::string CT_FILENAME;
			std::string CT_FILENAME2;
			std::string DOSE_FILENAME;

			if (argc > 1)
			{
				RTSTRUCT_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				CT_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTSTRUCT_FILENAME2 = argv[3];
			}

			if (argc > 4)
			{
				CT_FILENAME2 = argv[4];
			}

			if (argc > 5)
			{
				DOSE_FILENAME = argv[5];
			}

			/* structure set */

			StructureSetPointer rtStructureSet = io::virtuos::VirtuosFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str(), CT_FILENAME.c_str()).generateStructureSet();
			//2) test structure import
			CHECK_EQUAL(1, rtStructureSet->getNumberOfStructures());

			CHECK_EQUAL("TARGET_1", (rtStructureSet->getStructure(0))->getLabel());

			core::Structure myStructure = *(rtStructureSet->getStructure(0));
			CHECK_EQUAL(320, myStructure.getNumberOfEndpoints());

			PolygonSequenceType myPolygonSeq = myStructure.getStructureVector();
			CHECK_EQUAL(8, myPolygonSeq.size());

			PolygonType myPolygon = myPolygonSeq.at(0);

			CHECK_CLOSE(125.975, myPolygon.at(0).x(), errorConstant);
			CHECK_CLOSE(118.120, myPolygon.at(0).y(), errorConstant);
			CHECK_CLOSE(121.000, myPolygon.at(0).z(), errorConstant);

			CHECK_CLOSE(124.487, myPolygon.at(1).x(), errorConstant);
			CHECK_CLOSE(118.477, myPolygon.at(1).y(), errorConstant);
			CHECK_CLOSE(121.000, myPolygon.at(1).z(), errorConstant);

			CHECK_CLOSE(117.870, myPolygon.at(10).x(), errorConstant);
			CHECK_CLOSE(129.275, myPolygon.at(10).y(), errorConstant);
			CHECK_CLOSE(121.000, myPolygon.at(10).z(), errorConstant);

			CHECK_CLOSE(129.025, myPolygon.at(20).x(), errorConstant);
			CHECK_CLOSE(137.380, myPolygon.at(20).y(), errorConstant);
			CHECK_CLOSE(121.000, myPolygon.at(20).z(), errorConstant);

			CHECK_CLOSE(127.5, myPolygon.at(myPolygon.size() - 1).x(), errorConstant);
			CHECK_CLOSE(118, myPolygon.at(myPolygon.size() - 1).y(), errorConstant);
			CHECK_CLOSE(121.000, myPolygon.at(myPolygon.size() - 1).z(), errorConstant);
			//The original Virtuos contours are closed (last point = 1st point), but the ones used here do not have this double point.


			/* structure set2 */
			StructureSetPointer rtStructureSet2 = io::virtuos::VirtuosFileStructureSetGenerator(
			        RTSTRUCT_FILENAME2.c_str(), CT_FILENAME2.c_str()).generateStructureSet();
			//2) test structure import
			CHECK_EQUAL(9, rtStructureSet2->getNumberOfStructures());

			CHECK_EQUAL("BLASE", (rtStructureSet2->getStructure(0))->getLabel());
			CHECK_EQUAL("HUEFTK_LINKS", (rtStructureSet2->getStructure(3))->getLabel());
			CHECK_EQUAL("RAND", (rtStructureSet2->getStructure(5))->getLabel());
			CHECK_EQUAL("PTV", (rtStructureSet2->getStructure(8))->getLabel());

			core::Structure myStructure2 = *(rtStructureSet2->getStructure(8));
			CHECK_EQUAL(1653, myStructure2.getNumberOfEndpoints());

			PolygonSequenceType myPolygonSeq2 = myStructure2.getStructureVector();
			CHECK_EQUAL(27, myPolygonSeq2.size());

			PolygonType myPolygon2 = myPolygonSeq2.at(0);
			CHECK_CLOSE(244.511, myPolygon2.at(0).x(), errorConstant);
			CHECK_CLOSE(215.215, myPolygon2.at(0).y(), errorConstant);
			CHECK_CLOSE(149, myPolygon2.at(0).z(), errorConstant);

			CHECK_CLOSE(242.192, myPolygon2.at(1).x(), errorConstant);
			CHECK_CLOSE(215.215, myPolygon2.at(1).y(), errorConstant);
			CHECK_CLOSE(149, myPolygon2.at(1).z(), errorConstant);

			CHECK_CLOSE(221.318, myPolygon2.at(10).x(), errorConstant);
			CHECK_CLOSE(225.347, myPolygon2.at(10).y(), errorConstant);
			CHECK_CLOSE(149, myPolygon2.at(10).z(), errorConstant);

			CHECK_CLOSE(220.098, myPolygon2.at(20).x(), errorConstant);
			CHECK_CLOSE(239.873, myPolygon2.at(20).y(), errorConstant);
			CHECK_CLOSE(149, myPolygon2.at(20).z(), errorConstant);

			CHECK_CLOSE(245.854, myPolygon2.at(myPolygon2.size() - 1).x(), errorConstant);
			CHECK_CLOSE(215.581, myPolygon2.at(myPolygon2.size() - 1).y(), errorConstant);
			CHECK_CLOSE(149, myPolygon2.at(myPolygon2.size() - 1).z(), errorConstant);

			/* structure set2 on dose */
			StructureSetPointer rtStructureSet3 = io::virtuos::VirtuosFileStructureSetGenerator(
			        RTSTRUCT_FILENAME2.c_str(), DOSE_FILENAME.c_str()).generateStructureSet();
			//2) test structure import, should be equal to structure imported on CT-image
			CHECK_EQUAL(9, rtStructureSet3->getNumberOfStructures());

			CHECK_EQUAL("BLASE", (rtStructureSet3->getStructure(0))->getLabel());
			CHECK_EQUAL("HUEFTK_LINKS", (rtStructureSet3->getStructure(3))->getLabel());
			CHECK_EQUAL("RAND", (rtStructureSet3->getStructure(5))->getLabel());
			CHECK_EQUAL("PTV", (rtStructureSet3->getStructure(8))->getLabel());

			for (int s = 0; s < rtStructureSet3->getNumberOfStructures(); s++)
			{
				CHECK_EQUAL((rtStructureSet2->getStructure(s))->getLabel(),
				            (rtStructureSet3->getStructure(s))->getLabel());

				core::Structure myStructure3 = *(rtStructureSet3->getStructure(s));
				core::Structure myStructure2c = *(rtStructureSet2->getStructure(s));
				CHECK_EQUAL(myStructure2c.getNumberOfEndpoints(), myStructure3.getNumberOfEndpoints());
				PolygonSequenceType myPolygonSeq3 = myStructure3.getStructureVector();
				PolygonSequenceType myPolygonSeq2c = myStructure2c.getStructureVector();
				CHECK_EQUAL(myPolygonSeq2c.size(), myPolygonSeq3.size());

				for (int ps = 0; ps < myPolygonSeq3.size(); ps++)
				{
					PolygonType myPolygon2c = myPolygonSeq2c.at(ps);
					PolygonType myPolygon3 = myPolygonSeq3.at(ps);
					CHECK_CLOSE(myPolygon2c.at(0).x(), myPolygon3.at(0).x(), errorConstant);
					CHECK_CLOSE(myPolygon2c.at(0).y(), myPolygon3.at(0).y(), errorConstant);
					CHECK_CLOSE(myPolygon2c.at(0).z(), myPolygon3.at(0).z(), errorConstant);
				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

