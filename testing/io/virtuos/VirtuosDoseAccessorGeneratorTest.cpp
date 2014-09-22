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
#include "rttbVirtuosCubeinfoDoseAccessorGenerator.h"
#include "rttbVirtuosDoseFileDoseAccessorGenerator.h"
#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "../rttbDoseAccessorTester.h"


namespace rttb
{

	namespace testing
	{

		/*! @brief VirtuosDoseAccessorGeneratorTest - test the accessor generator generateDoseAccessor() of virtuos data
				1) test virtuos accessor generator with only dose file
				2) test virtuos accessor generator with dose and plan file
				3) test virtuos accessor generator with cubeinfo
		*/

		int VirtuosDoseAccessorGeneratorTest(int argc, char* argv[])
		{

			PREPARE_DEFAULT_TEST_REPORTING;
			//           1: plan  file name (virtuos)		.../testing/data/Virtuos/prostate_ac/prostate_ac101.pln
			//           2: dose1 file name (virtuos)		.../testing/data/Virtuos/prostate_ac/prostate_ac101.dos.gz
			//           3: dose2 file name (trip):		.../testing/data/Virtuos/prostate_ac/prostate_ac101.dos
			// WARNING: Test will fail if dose2 does not contain the same dose as dose1!

			std::string RTPLAN_FILENAME;
			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;

			if (argc > 1)
			{
				RTPLAN_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTDOSE2_FILENAME = argv[3];
			}

			//1) test virtuos accessor generator with only dose file
			CHECK_NO_THROW(io::virtuos::VirtuosDoseFileDoseAccessorGenerator(RTDOSE_FILENAME.c_str(), 0,
			               0).generateDoseAccessor());
			CHECK_THROW_EXPLICIT(
			    io::virtuos::VirtuosDoseFileDoseAccessorGenerator("test.dos.gz", 0, 0).generateDoseAccessor(),
			    core::InvalidParameterException);



			//2) test virtuos accessor generator with dose and plan file
			CHECK_NO_THROW(io::virtuos::VirtuosPlanFileDoseAccessorGenerator(RTDOSE_FILENAME.c_str(), ""));
			CHECK_NO_THROW(io::virtuos::VirtuosPlanFileDoseAccessorGenerator("myDose.dos.gz",
			               RTPLAN_FILENAME.c_str()));

			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosPlanFileDoseAccessorGenerator(RTDOSE_FILENAME.c_str(),
			                     "").generateDoseAccessor(), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosPlanFileDoseAccessorGenerator("",
			                     RTPLAN_FILENAME.c_str()).generateDoseAccessor(), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosPlanFileDoseAccessorGenerator(RTDOSE_FILENAME.c_str(),
			                     "myPlanfile.pln").generateDoseAccessor(), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosPlanFileDoseAccessorGenerator("myDose.dos.gz",
			                     RTPLAN_FILENAME.c_str()).generateDoseAccessor(), core::InvalidParameterException);

			CHECK_NO_THROW(io::virtuos::VirtuosPlanFileDoseAccessorGenerator(RTDOSE_FILENAME.c_str(),
			               RTPLAN_FILENAME.c_str()).generateDoseAccessor());


			std::string testFileName = RTPLAN_FILENAME;
			size_t gzPosition = testFileName.find(".pln");

			if (gzPosition != std::string::npos)
			{
				testFileName.erase(gzPosition, testFileName.length());
			}

			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosPlanFileDoseAccessorGenerator(RTDOSE_FILENAME.c_str(),
			                     testFileName).generateDoseAccessor(), core::InvalidParameterException);

			testFileName = RTDOSE_FILENAME;
			gzPosition = testFileName.find(".dos");

			if (gzPosition != std::string::npos)
			{
				testFileName.erase(gzPosition, testFileName.length());
			}

			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosPlanFileDoseAccessorGenerator(testFileName,
			                     RTPLAN_FILENAME.c_str()).generateDoseAccessor(), core::InvalidParameterException);

			//3) test virtuos accessor generator with cubeinfo
			Cubeinfo** pPointerOnVirtuosCube = new Cubeinfo*;
			*pPointerOnVirtuosCube = create_cubeinfo(0);
			CHECK_NO_THROW(io::virtuos::VirtuosCubeinfoDoseAccessorGenerator(*pPointerOnVirtuosCube));
			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosCubeinfoDoseAccessorGenerator(
			                         *pPointerOnVirtuosCube).generateDoseAccessor(), core::InvalidDoseException);

			nc_init_cubeinfo(*pPointerOnVirtuosCube);
			CHECK_THROW_EXPLICIT(io::virtuos::VirtuosCubeinfoDoseAccessorGenerator(
			                         *pPointerOnVirtuosCube).generateDoseAccessor(), core::InvalidDoseException);

			opencube(RTDOSE2_FILENAME.c_str() , *pPointerOnVirtuosCube);
			CHECK_NO_THROW(io::virtuos::VirtuosCubeinfoDoseAccessorGenerator(
			                   *pPointerOnVirtuosCube).generateDoseAccessor());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

