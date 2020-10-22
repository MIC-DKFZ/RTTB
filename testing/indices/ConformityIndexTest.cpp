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

#include "litCheckMacros.h"
#include "rttbDoseIndex.h"
#include "rttbDVHSet.h"
#include "rttbBaseType.h"
#include "rttbNullPointerException.h"
#include "rttbConformityIndex.h"
#include "rttbException.h"
#include "rttbInvalidParameterException.h"
#include "rttbDVHXMLFileReader.h"

#include <math.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

namespace rttb
{
	namespace testing
	{
		/*! @brief Test of ConformityIndex.
		test dvh: deltaV 0.125, deltaD 0.5
		1. dvh TV: number of voxels 2900,  maximum dose bin 133, dose bin 127~133
		2. dvh HT1: number of voxels 5410, maximum dose bin 40, dose bin 0~2,40
		3. dvh HT2: number of voxels 10210, maximum dose bin 50, dose bin 0~2,50
		4. dvh HT3: number of voxels 1210, maximum dose bin 70, dose bin 0~2,70

		Test if calculation in new architecture returns similar results to the
		original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/
		int ConformityIndexTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//ARGUMENTS: 1: ptv dvh file name
			//           2: normal tissue 1 dvh file name
			//           3: normal tissue 2 dvh file name
			//           4: normal tissue 3 dvh file name

			std::string DVH_FILENAME_PTV;
			std::string DVH_FILENAME_NT1;
			std::string DVH_FILENAME_NT2;
			std::string DVH_FILENAME_NT3;


			if (argc > 1)
			{
				DVH_FILENAME_PTV = argv[1];
			}

			if (argc > 2)
			{
				DVH_FILENAME_NT1 = argv[2];
			}

			if (argc > 3)
			{
				DVH_FILENAME_NT2 = argv[3];
			}

			if (argc > 4)
			{
				DVH_FILENAME_NT3 = argv[4];
			}


			/*test dvh: deltaV 0.125, deltaD 0.5*/
			/*dvh TV: number of voxels 2900,  maximum dose bin 133, dose bin 127~133*/
			rttb::io::other::DVHXMLFileReader dvhReader = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_PTV);
			rttb::core::DVH dvhPTV = *(dvhReader.generateDVH());
			/*dvh HT1: number of voxels 5410, maximum dose bin 40, dose bin 0~2,40*/
			rttb::io::other::DVHXMLFileReader dvhReader1 = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_NT1);
			core::DVH dvhNT1 = *(dvhReader1.generateDVH());
			/*dvh HT2: number of voxels 10210, maximum dose bin 50, dose bin 0~2,50*/
			rttb::io::other::DVHXMLFileReader dvhReader2 = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_NT2);
			core::DVH dvhNT2 = *(dvhReader2.generateDVH());
			/*dvh HT3: number of voxels 1210, maximum dose bin 70, dose bin 0~2,70*/
			rttb::io::other::DVHXMLFileReader dvhReader3 = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_NT3);
			core::DVH dvhNT3 = *(dvhReader3.generateDVH());

			std::vector<core::DVH> dvhTVSet;
			std::vector<core::DVH> dvhHTSet;
			dvhTVSet.push_back(dvhPTV);
			dvhHTSet.push_back(dvhNT1);
			dvhHTSet.push_back(dvhNT2);
			dvhHTSet.push_back(dvhNT3);

			::boost::shared_ptr<core::DVHSet> dvhSetPtr = ::boost::make_shared<core::DVHSet>(dvhTVSet, dvhHTSet,
			        "testStrSet", dvhPTV.getDoseID());

			/*test exception*/
			::boost::shared_ptr<core::DVHSet> dvhSetNullPtr;
			CHECK_THROW_EXPLICIT(indices::ConformityIndex(dvhSetNullPtr, 0), core::InvalidParameterException);

			/*test exception for invalid reference dose*/
			CHECK_THROW_EXPLICIT(indices::ConformityIndex(dvhSetPtr, 100), core::InvalidParameterException);

			/*test index calculation*/

			/*ConformityIndex */
			indices::ConformityIndex ci = indices::ConformityIndex(dvhSetPtr, 30);
			CHECK_CLOSE(362.5 / 363.75, ci.getValue(), errorConstant);

			ci.setDoseReference(65);
			CHECK_CLOSE(2300 / 2900.0, ci.getValue(), errorConstant); //ref dose: 65->ci=2300/2900*1*1*1

			CHECK_EQUAL(ci.getValue(), ci.getValueAt(0));
			CHECK_THROW_EXPLICIT(ci.getValueAt(1), core::InvalidParameterException);

			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb