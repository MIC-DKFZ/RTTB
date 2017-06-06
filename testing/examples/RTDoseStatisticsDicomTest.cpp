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
// @version $Revision: 1328 $ (last changed revision)
// @date $Date: 2016-04-22 09:50:01 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbDoseStatistics.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace testing
	{


		/*! @brief RTDoseStatisticsTest. Max, min, mean, standard deviation, variance, Vx, Dx, MOHx, MOCx, MaxOHx,
		MinOCx are tested. Test if calculation in new architecture returns similar results to the original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!*/

		int RTDoseStatisticsDicomTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;
			std::string RTDOSE_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}
			else
			{
				std::cout << "at least one arguments required for RTDoseStatisticsDicomTest" << std::endl;
				return -1;
			}

			double expectedVal = 5.64477e-005;
			double volume = 24120.;

			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericDoseIterator::DoseIteratorPointer DoseIteratorPointer;
			typedef algorithms::DoseStatisticsCalculator::ResultListPointer ResultListPointer;

			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//create corresponding DoseIterator
			::boost::shared_ptr<core::GenericDoseIterator> spDoseIteratorTmp =
			    ::boost::make_shared<core::GenericDoseIterator>(doseAccessor1);
			DoseIteratorPointer spDoseIterator(spDoseIteratorTmp);
			rttb::algorithms::DoseStatisticsCalculator doseStatisticsCalculator(spDoseIterator);

			std::vector<double> precomputedDoseValues;
			precomputedDoseValues.push_back(0);
			precomputedDoseValues.push_back(0.5);
			precomputedDoseValues.push_back(1);
			std::vector<double> precomputedVolumeValues;
			precomputedVolumeValues.push_back(20000 / volume);
			precomputedVolumeValues.push_back(1);

			rttb::algorithms::DoseStatistics::DoseStatisticsPointer doseStatistics =
			    doseStatisticsCalculator.calculateDoseStatistics(precomputedDoseValues, precomputedVolumeValues);

			CHECK_CLOSE(doseStatistics->getMean(), expectedVal, errorConstant);
			CHECK_CLOSE(doseStatistics->getStdDeviation(), 0, errorConstant);
			CHECK_CLOSE(doseStatistics->getVariance(), 0, errorConstant);

			double dummy;
			DoseTypeGy vx = doseStatistics->getVx(expectedVal, true, dummy);
			CHECK_EQUAL(vx, doseStatistics->getVx(0));
			CHECK_CLOSE(expectedVal, doseStatistics->getDx().getValue(vx), reducedErrorConstant);

			CHECK_CLOSE(doseStatistics->getMaximum(), expectedVal, errorConstant);

			CHECK_CLOSE(doseStatistics->getMinimum(), expectedVal, errorConstant);
			ResultListPointer minListPtr = doseStatistics->getMinimumVoxelPositions();
			ResultListPointer maxListPtr = doseStatistics->getMaximumVoxelPositions();
			CHECK_EQUAL(maxListPtr->size(), 10);
			CHECK_EQUAL(minListPtr->size(), 10);

			CHECK_CLOSE(doseStatistics->getDx().getValue(24120), doseStatistics->getMinimum(), 0.001);
			CHECK_CLOSE(doseStatistics->getMOHx(24120), doseStatistics->getMean(), reducedErrorConstant);
			CHECK_CLOSE(doseStatistics->getMOCx(20000), doseStatistics->getMean(), reducedErrorConstant);
			CHECK_CLOSE(doseStatistics->getMinOCx(20000), doseStatistics->getMean(), reducedErrorConstant);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb
