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

#include "rttbBaseType.h"
#include "rttbDoseStatistics.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{
	namespace testing
	{

		typedef rttb::algorithms::DoseStatistics::ResultListPointer ResultListPointer;
		typedef rttb::algorithms::DoseStatistics::DoseToVolumeFunctionType DoseToVolumeFunctionType;
		typedef rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType VolumeToDoseFunctionType;

		/*! @brief DoseStatisticsTest - test the API of DoseStatistics
			1) test constructors
			2) test setters
			3) test getters of complex statistics (with stored key and without stored key)
		*/

		int DoseStatisticsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			DoseStatisticType minimum = 1.0;
			DoseStatisticType mean = 5.5;
			DoseStatisticType maximum = 108.2;
			DoseStatisticType stdDeviation = 10.1;
			unsigned int numVoxels = 100000;
			VolumeType volume = numVoxels * (0.5 * 0.5 * 0.5);

			std::vector<std::pair<DoseTypeGy, VoxelGridID> > minVoxels;
			std::vector<std::pair<DoseTypeGy, VoxelGridID> > maxVoxels;

			minVoxels.push_back(std::make_pair(1.0, 11));
			minVoxels.push_back(std::make_pair(1.0, 22));
			minVoxels.push_back(std::make_pair(1.0, 33));
			minVoxels.push_back(std::make_pair(1.0, 44));

			maxVoxels.push_back(std::make_pair(108.2, 5));
			maxVoxels.push_back(std::make_pair(108.2, 6));
			maxVoxels.push_back(std::make_pair(108.2, 7));
			maxVoxels.push_back(std::make_pair(108.2, 8));

			ResultListPointer resultsMinVoxels = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >(minVoxels);
			ResultListPointer resultsMaxVoxels = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >(maxVoxels);

			DoseToVolumeFunctionType Vx;
			Vx.insert(std::make_pair(1.1, 1000));
			Vx.insert(std::make_pair(106.9, 99000));

			VolumeToDoseFunctionType Dx;
			Dx.insert(std::make_pair(1000, 1.1));
			Dx.insert(std::make_pair(99000, 106.9));

			VolumeToDoseFunctionType MOHx;
			MOHx.insert(std::make_pair(1000, 5));
			MOHx.insert(std::make_pair(99000, 105.5));

			VolumeToDoseFunctionType MOCx;
			MOCx.insert(std::make_pair(1000, 10));
			MOCx.insert(std::make_pair(99000, 99));

			VolumeToDoseFunctionType MaxOHx;
			MaxOHx.insert(std::make_pair(1000, 40));
			MaxOHx.insert(std::make_pair(99000, 98.3));

			VolumeToDoseFunctionType MinOCx;
			MinOCx.insert(std::make_pair(1000, 25.5));
			MinOCx.insert(std::make_pair(99000, 102.7));


			//1) test constructors

			CHECK_NO_THROW(rttb::algorithms::DoseStatistics aDoseStatistic(minimum, maximum, mean, stdDeviation, numVoxels,
			               volume));

			rttb::algorithms::DoseStatistics aDoseStatistic(minimum, maximum, mean, stdDeviation, numVoxels, volume);
			CHECK_EQUAL(aDoseStatistic.getMinimum(), minimum);
			CHECK_EQUAL(aDoseStatistic.getMaximum(), maximum);
			CHECK_EQUAL(aDoseStatistic.getMean(), mean);
			CHECK_EQUAL(aDoseStatistic.getStdDeviation(), stdDeviation);
			CHECK_EQUAL(aDoseStatistic.getVariance(), stdDeviation * stdDeviation);
			CHECK_EQUAL(aDoseStatistic.getNumberOfVoxels(), numVoxels);
			CHECK_EQUAL(aDoseStatistic.getVolume(), volume);

			//check default values for unset complex values
			CHECK_EQUAL(aDoseStatistic.getMaximumPositions()->empty(), true);
			CHECK_EQUAL(aDoseStatistic.getMinimumPositions()->empty(), true);
			CHECK_EQUAL(aDoseStatistic.getAllDx().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getAllVx().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getAllMOHx().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getAllMOCx().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getAllMaxOHx().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getAllMinOCx().empty(), true);

			CHECK_NO_THROW(rttb::algorithms::DoseStatistics aDoseStatisticComplex(minimum, maximum, mean, stdDeviation, numVoxels,
			               volume,
			               resultsMaxVoxels, resultsMinVoxels, Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx));
			rttb::algorithms::DoseStatistics aDoseStatisticComplex(minimum, maximum, mean, stdDeviation, numVoxels, volume,
			        resultsMaxVoxels, resultsMinVoxels, Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx);

			CHECK_EQUAL(aDoseStatisticComplex.getMaximumPositions(), resultsMaxVoxels);
			CHECK_EQUAL(aDoseStatisticComplex.getMinimumPositions(), resultsMinVoxels);
			CHECK_EQUAL(aDoseStatisticComplex.getAllDx() == Dx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getAllVx() == Vx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getAllMOHx() == MOHx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getAllMOCx() == MOCx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getAllMaxOHx() == MaxOHx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getAllMinOCx() == MinOCx, true);

			//2) test setters (only complex statistics have setters)
			CHECK_NO_THROW(aDoseStatistic.setMaximumVoxelPositions(resultsMaxVoxels));
			CHECK_NO_THROW(aDoseStatistic.setMinimumVoxelPositions(resultsMinVoxels));
			CHECK_NO_THROW(aDoseStatistic.setDx(Dx));
			CHECK_NO_THROW(aDoseStatistic.setVx(Vx));
			CHECK_NO_THROW(aDoseStatistic.setMOHx(MOHx));
			CHECK_NO_THROW(aDoseStatistic.setMOCx(MOCx));
			CHECK_NO_THROW(aDoseStatistic.setMaxOHx(MaxOHx));
			CHECK_NO_THROW(aDoseStatistic.setMinOCx(MinOCx));

			CHECK_EQUAL(aDoseStatistic.getMaximumPositions(), resultsMaxVoxels);
			CHECK_EQUAL(aDoseStatistic.getMinimumPositions(), resultsMinVoxels);
			CHECK_EQUAL(aDoseStatistic.getAllDx() == Dx, true);
			CHECK_EQUAL(aDoseStatistic.getAllVx() == Vx, true);
			CHECK_EQUAL(aDoseStatistic.getAllMOHx() == MOHx, true);
			CHECK_EQUAL(aDoseStatistic.getAllMOCx() == MOCx, true);
			CHECK_EQUAL(aDoseStatistic.getAllMaxOHx() == MaxOHx, true);
			CHECK_EQUAL(aDoseStatistic.getAllMinOCx() == MinOCx, true);

			//3) test getters of complex statistics(with stored key and without stored key)
			//getAll*() already tested in (2)
			Vx.clear();
			Vx.insert(std::make_pair(1.1, 1000));
			Vx.insert(std::make_pair(5.0, 2300));
			Vx.insert(std::make_pair(90, 90500));
			Vx.insert(std::make_pair(107, 99000));

			Dx.clear();
			Dx.insert(std::make_pair(1000, 1.1));
			Dx.insert(std::make_pair(2000, 2.0));
			Dx.insert(std::make_pair(5000, 10.8));
			Dx.insert(std::make_pair(90000, 89.5));
			Dx.insert(std::make_pair(98000, 104.4));
			Dx.insert(std::make_pair(99000, 106.9));

			rttb::algorithms::DoseStatistics aDoseStatisticNewValues(minimum, maximum, mean, stdDeviation, numVoxels, volume);
			aDoseStatisticNewValues.setDx(Dx);
			aDoseStatisticNewValues.setVx(Vx);

			CHECK_NO_THROW(aDoseStatisticNewValues.getVx(1.1));
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx(90));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx(1000));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx(98000));

			CHECK_EQUAL(aDoseStatisticNewValues.getVx(1.1), Vx.find(1.1)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getVx(90), Vx.find(90)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getDx(1000), Dx.find(1000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getDx(98000), Dx.find(98000)->second);

			//test if key-value combination NOT in map
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getDx(1001), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getVx(10), core::DataNotAvailableException);

			double closestDxKey, closestVxKey;
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx(900, true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx(99001, true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx(10, true, closestVxKey));
			CHECK_EQUAL(aDoseStatisticNewValues.getDx(900, true, closestDxKey), Dx.find(1000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getDx(99001, true, closestDxKey), Dx.find(99000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getVx(10, true, closestVxKey), Vx.find(5.0)->second);
			CHECK_EQUAL(closestDxKey, 99000);
			CHECK_EQUAL(closestVxKey, 5);

			//equal distance to two values. First value is returned.
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx(1500, true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx(98.5, true, closestVxKey));
			CHECK_EQUAL(aDoseStatisticNewValues.getDx(1500, true, closestDxKey), Dx.find(1000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getVx(98.5, true, closestVxKey), Vx.find(90.0)->second);
			CHECK_EQUAL(closestDxKey, 1000);
			CHECK_EQUAL(closestVxKey, 90.0);

			double dummy;
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMinOCx(25), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMOHx(9999), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMinOCx(25, true, dummy), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMOHx(9999, true, dummy), core::DataNotAvailableException);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
