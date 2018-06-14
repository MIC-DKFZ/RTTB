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
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbDVH.h"
#include "rttbDvhBasedModels.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace testing
	{

		typedef core::DVH::DataDifferentialType DataDifferentialType;

		/*! @brief DvhBasedModelsTest.
		1) Test bed und lqed2
		*/
		int DvhBasedModelsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//1) test calcBEDDVH and calcLQED2DVH
			//generate artificial DVH and corresponding statistical values
			DoseTypeGy binSize = DoseTypeGy(0.1);
			DoseVoxelVolumeType voxelVolume = 8;
			const IDType structureID = "myStructure";
			const IDType doseID = "myDose";
			const IDType voxelizationID = "myVoxelization";
			DataDifferentialType aDataDifferential;
			std::vector<double> bedVector;
			std::vector<double> lqed2Vector;
			int numberOfFractions = 2;
			double alpha_beta = 10;

			for (int i = 0; i < 100; i++)
			{
				double volume = DoseCalcType((double(rand()) / RAND_MAX) * 1000);
				double dose = (i + 0.5) * binSize;
				aDataDifferential.push_back(volume);
				bedVector.push_back(dose * (1 + dose / (numberOfFractions * alpha_beta)));
				lqed2Vector.push_back(dose * ((alpha_beta + (dose / numberOfFractions)) / (alpha_beta + 2)));
			}

			core::DVH myDVH(aDataDifferential, binSize, voxelVolume, structureID, doseID, voxelizationID);
			core::DVH::Pointer dvhPtr = boost::make_shared<core::DVH>(myDVH);

			CHECK_THROW_EXPLICIT(rttb::models::calcBEDDVH(dvhPtr, 0, 10), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(rttb::models::calcBEDDVH(dvhPtr, 10, -1), core::InvalidParameterException);
			CHECK_NO_THROW(rttb::models::calcBEDDVH(dvhPtr, 10, 10));
			CHECK_EQUAL(rttb::models::calcBEDDVH(dvhPtr, 2, 10).size(), myDVH.getDataDifferential().size());
			rttb::models::BEDDVHType bedDVH = rttb::models::calcBEDDVH(dvhPtr, numberOfFractions, alpha_beta);

			CHECK_THROW_EXPLICIT(rttb::models::calcLQED2DVH(dvhPtr, 1, 10), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(rttb::models::calcLQED2DVH(dvhPtr, 10, -1), core::InvalidParameterException);
			CHECK_NO_THROW(rttb::models::calcLQED2DVH(dvhPtr, 10, 10, true));
			CHECK_EQUAL(rttb::models::calcLQED2DVH(dvhPtr, 2, 10).size(), myDVH.getDataDifferential().size());
			rttb::models::BEDDVHType lqed2DVH = rttb::models::calcLQED2DVH(dvhPtr, numberOfFractions,
			                                    alpha_beta);

			//check the calculation
			rttb::models::BEDDVHType::iterator itBED, itLQED2;
			std::vector<double>::iterator itBEDVec, itLQED2Vec;
			DataDifferentialType::iterator itDiff;

			for (itBED = bedDVH.begin(), itLQED2 = lqed2DVH.begin(), itBEDVec = bedVector.begin(),
			     itLQED2Vec = lqed2Vector.begin(), itDiff = aDataDifferential.begin();
			     itBED != bedDVH.end(), itLQED2 != lqed2DVH.end(), itBEDVec != bedVector.end(),
			     itLQED2Vec != lqed2Vector.end(), itDiff != aDataDifferential.end();
			     ++itBED, ++itLQED2, ++itBEDVec, ++itLQED2Vec, ++itDiff)
			{

				//check volume
				CHECK_EQUAL(*itDiff, (*itBED).second);
				CHECK_EQUAL((*itBED).second, (*itLQED2).second);

				//check bed
				CHECK_EQUAL(*itBEDVec, (*itBED).first);

				//check lqed2
				CHECK_EQUAL(*itLQED2Vec, (*itLQED2).first);
			}

			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb