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

#include "litCheckMacros.h"
#include "rttbDoseIndex.h"
#include "rttbDVHSet.h"
#include "rttbDVHTxtFileReader.h"
#include "rttbBaseType.h"
#include "rttbNullPointerException.h"
#include "rttbConformalIndex.h"
#include "rttbConformationNumber.h"
#include "rttbConformityIndex.h"
#include "rttbCoverageIndex.h"
#include "rttbHomogeneityIndex.h"
#include "rttbException.h"
#include "rttbInvalidParameterException.h"

#include <math.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>


namespace rttb
{
	namespace testing
	{
		/*! @brief DoseIndexTest. ConformationNumber ConformalIndex ConformityIndex CoverageIndex HomogeneityIndex are tested.
		test dvh: deltaV 0.125, deltaD 0.5
		1. dvh TV: number of voxels 2900,  maximum dose bin 133, dose bin 127~133
		2. dvh HT1: number of voxels 5410, maximum dose bin 40, dose bin 0~2,40
		3. dvh HT2: number of voxels 10210, maximum dose bin 50, dose bin 0~2,50
		4. dvh HT3: number of voxels 1210, maximum dose bin 70, dose bin 0~2,70

		Test if calculation in new architecture returns similar results to the
		original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/
		int RTDoseIndexTest(int argc, char* argv[])
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
			rttb::io::other::DVHTxtFileReader dvhReader = rttb::io::other::DVHTxtFileReader(DVH_FILENAME_PTV);
			rttb::core::DVH dvhPTV = *(dvhReader.generateDVH());
			/*dvh HT1: number of voxels 5410, maximum dose bin 40, dose bin 0~2,40*/
			rttb::io::other::DVHTxtFileReader dvhReader1 = rttb::io::other::DVHTxtFileReader(DVH_FILENAME_NT1);
			core::DVH dvhNT1 = *(dvhReader1.generateDVH());
			/*dvh HT2: number of voxels 10210, maximum dose bin 50, dose bin 0~2,50*/
			rttb::io::other::DVHTxtFileReader dvhReader2 = rttb::io::other::DVHTxtFileReader(DVH_FILENAME_NT2);
			core::DVH dvhNT2 = *(dvhReader2.generateDVH());
			/*dvh HT3: number of voxels 1210, maximum dose bin 70, dose bin 0~2,70*/
			rttb::io::other::DVHTxtFileReader dvhReader3 = rttb::io::other::DVHTxtFileReader(DVH_FILENAME_NT3);
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
			CHECK_THROW_EXPLICIT(indices::ConformalIndex(dvhSetNullPtr, 0), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(indices::ConformationNumber(dvhSetNullPtr, 0),
			                     core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(indices::ConformityIndex(dvhSetNullPtr, 0), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(indices::CoverageIndex(dvhSetNullPtr, 0), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(indices::HomogeneityIndex(dvhSetNullPtr, 0), core::InvalidParameterException);

			/*test exception for invalid reference dose*/
			CHECK_THROW_EXPLICIT(indices::ConformalIndex(dvhSetPtr, 100), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(indices::ConformationNumber(dvhSetPtr, 100), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(indices::ConformityIndex(dvhSetPtr, 100), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(indices::HomogeneityIndex(dvhSetPtr, 0), core::InvalidParameterException);

			/*test index calculation*/

			/*RTConformationNumber */
			//PTV covered by reference dose 30 = the whole PTV =362.5; Volume of the referece 30=362.5+1.25
			indices::ConformationNumber cn = indices::ConformationNumber(dvhSetPtr, 30);
			//check if values are close. Equality is only achieved with double precission.
			CHECK_CLOSE(362.5 / 363.75, cn.getValue(), errorConstant);

			//cn==1*TV0/V0=362.5/2466.25
			cn.setDoseReference(0);
			CHECK_CLOSE(362.5 / 2466.25, cn.getValue(), errorConstant);

			cn.setDoseReference(65);
			CHECK_CLOSE(2300 / 2900.0, cn.getValue(),
			            errorConstant); //ref dose: 65 -> TVref=Vref -> cn=TVref/TV=2300/2900

			CHECK_EQUAL(cn.getValue(), cn.getValueAt(0));
			CHECK_THROW_EXPLICIT(cn.getValueAt(1), core::InvalidParameterException);

			/*ConformalIndex */
			//HT 1 covered by ref=HT2 covered by ref=0, HT3 covered by ref=1.25
			indices::ConformalIndex coin = indices::ConformalIndex(dvhSetPtr, 30);
			CHECK_CLOSE((362.5 / 363.75) * (1 - 1.25 / 151.25), coin.getValue(), errorConstant);

			coin.setDoseReference(0);
			CHECK_EQUAL(0, coin.getValue());

			coin.setDoseReference(65);
			CHECK_CLOSE(2300 / 2900.0, coin.getValue(),
			            errorConstant); //ref dose: 65 -> Vref=0 for all HT -> cn=cn*(1-0)=cn

			CHECK_EQUAL(coin.getValue(), coin.getValueAt(0));
			CHECK_THROW_EXPLICIT(coin.getValueAt(1), core::InvalidParameterException);

			/*ConformityIndex */
			indices::ConformityIndex ci = indices::ConformityIndex(dvhSetPtr, 30);
			CHECK_CLOSE(362.5 / 363.75, ci.getValue(), errorConstant);

			ci.setDoseReference(65);
			CHECK_CLOSE(2300 / 2900.0, ci.getValue(), errorConstant); //ref dose: 65->ci=2300/2900*1*1*1

			CHECK_EQUAL(ci.getValue(), ci.getValueAt(0));
			CHECK_THROW_EXPLICIT(ci.getValueAt(1), core::InvalidParameterException);

			/*CoverageIndex*/
			indices::CoverageIndex coverageI = indices::CoverageIndex(dvhSetPtr, 30);
			CHECK_CLOSE(362.5 / 362.5, coverageI.getValue(), errorConstant); //ref dose: 30 -> coverage index=1

			coverageI.setDoseReference(65);
			CHECK_CLOSE(2300 / 2900.0, coverageI.getValue(),
			            errorConstant); //ref dose: 65->coverage index=2300/2900

			CHECK_EQUAL(coverageI.getValue(), coverageI.getValueAt(0));
			CHECK_THROW_EXPLICIT(coverageI.getValueAt(1), core::InvalidParameterException);


			/*HomogeneityIndex TV max: 133*0.5=66.5, TV min: 127*0.5=63.5 -> hi=(66.5-63.5)/30*/
			indices::HomogeneityIndex hi = indices::HomogeneityIndex(dvhSetPtr, 30);
			CHECK_CLOSE(3 / 30.0, hi.getValue(), errorConstant);

			hi.setDoseReference(65);
			CHECK_CLOSE(3 / 65.0, hi.getValue(), errorConstant);

			CHECK_EQUAL(hi.getValue(), hi.getValueAt(0));
			CHECK_THROW_EXPLICIT(hi.getValueAt(1), core::InvalidParameterException);


			RETURN_AND_REPORT_TEST_SUCCESS;


		}

	}//testing
}//rttb