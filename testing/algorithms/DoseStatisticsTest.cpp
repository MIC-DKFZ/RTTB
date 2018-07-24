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

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDoseStatistics.h"
#include "rttbDataNotAvailableException.h"

#include "rttbVolumeToDoseMeasureCollection.h"

namespace rttb
{
	namespace testing
	{

		typedef rttb::algorithms::DoseStatistics::ResultListPointer ResultListPointer;

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

			ResultListPointer resultsMinVoxels =
			    boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >(minVoxels);
			ResultListPointer resultsMaxVoxels =
			    boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >(maxVoxels);

			algorithms::DoseToVolumeMeasureCollection Vx(algorithms::DoseToVolumeMeasureCollection::Vx, maximum);
			Vx.insertValue(1.1, 1000);
			Vx.insertValue(106.9, 99000);

			algorithms::VolumeToDoseMeasureCollection Dx(algorithms::VolumeToDoseMeasureCollection::Dx, volume);
			Dx.insertValue(1000, 1.1);
			Dx.insertValue(99000, 106.9);

			algorithms::VolumeToDoseMeasureCollection MOHx(algorithms::VolumeToDoseMeasureCollection::MOHx, volume);
			MOHx.insertValue(1000, 5);
			MOHx.insertValue(99000, 105.5);
			
			algorithms::VolumeToDoseMeasureCollection MOCx(algorithms::VolumeToDoseMeasureCollection::MOCx, volume);
			MOCx.insertValue(1000, 10);
			MOCx.insertValue(99000, 99);

			algorithms::VolumeToDoseMeasureCollection MaxOHx(algorithms::VolumeToDoseMeasureCollection::MaxOHx, volume);
			MaxOHx.insertValue(1000, 40);
			MaxOHx.insertValue(99000, 98.3);

			algorithms::VolumeToDoseMeasureCollection MinOCx(algorithms::VolumeToDoseMeasureCollection::MinOCx, volume);
			MinOCx.insertValue(1000, 25.5);
			MinOCx.insertValue(99000, 102.7);


			//1) test constructors

			CHECK_NO_THROW(rttb::algorithms::DoseStatistics aDoseStatistic(minimum, maximum, mean, stdDeviation,
			               numVoxels,
			               volume));

			rttb::algorithms::DoseStatistics aDoseStatistic(minimum, maximum, mean, stdDeviation, numVoxels,
			        volume);
			CHECK_EQUAL(aDoseStatistic.getMinimum(), minimum);
			CHECK_EQUAL(aDoseStatistic.getMaximum(), maximum);
			CHECK_EQUAL(aDoseStatistic.getMean(), mean);
			CHECK_EQUAL(aDoseStatistic.getStdDeviation(), stdDeviation);
			CHECK_EQUAL(aDoseStatistic.getVariance(), stdDeviation * stdDeviation);
			CHECK_EQUAL(aDoseStatistic.getNumberOfVoxels(), numVoxels);
			CHECK_EQUAL(aDoseStatistic.getVolume(), volume);

			//check default values for unset complex values
			CHECK_EQUAL(aDoseStatistic.getMaximumVoxelPositions()->empty(), true);
			CHECK_EQUAL(aDoseStatistic.getMinimumVoxelPositions()->empty(), true);
			CHECK_EQUAL(aDoseStatistic.getDx().getAllValues().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getVx().getAllValues().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getMOHx().getAllValues().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getMOCx().getAllValues().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getMaxOHx().getAllValues().empty(), true);
			CHECK_EQUAL(aDoseStatistic.getMinOCx().getAllValues().empty(), true);

			CHECK_NO_THROW(rttb::algorithms::DoseStatistics aDoseStatisticComplex(minimum, maximum, mean,
			               stdDeviation, numVoxels,
			               volume,
			               resultsMaxVoxels, resultsMinVoxels, Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx));
			rttb::algorithms::DoseStatistics aDoseStatisticComplex(minimum, maximum, mean, stdDeviation,
			        numVoxels, volume,
			        resultsMaxVoxels, resultsMinVoxels, Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx);

			CHECK_EQUAL(aDoseStatisticComplex.getMaximumVoxelPositions(), resultsMaxVoxels);
			CHECK_EQUAL(aDoseStatisticComplex.getMinimumVoxelPositions(), resultsMinVoxels);
			CHECK_EQUAL(aDoseStatisticComplex.getDx() == Dx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getVx() == Vx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getMOHx() == MOHx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getMOCx() == MOCx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getMaxOHx() == MaxOHx, true);
			CHECK_EQUAL(aDoseStatisticComplex.getMinOCx() == MinOCx, true);

			//2) test setters (only complex statistics have setters)
			CHECK_NO_THROW(aDoseStatistic.setMaximumVoxelPositions(resultsMaxVoxels));
			CHECK_NO_THROW(aDoseStatistic.setMinimumVoxelPositions(resultsMinVoxels));
			CHECK_NO_THROW(aDoseStatistic.setDx(::boost::make_shared<algorithms::VolumeToDoseMeasureCollection>(Dx)));
			CHECK_NO_THROW(aDoseStatistic.setVx(::boost::make_shared<algorithms::DoseToVolumeMeasureCollection>(Vx)));
			CHECK_NO_THROW(aDoseStatistic.setMOHx(::boost::make_shared<algorithms::VolumeToDoseMeasureCollection>(MOHx)));
			CHECK_NO_THROW(aDoseStatistic.setMOCx(::boost::make_shared<algorithms::VolumeToDoseMeasureCollection>(MOCx)));
			CHECK_NO_THROW(aDoseStatistic.setMaxOHx(::boost::make_shared<algorithms::VolumeToDoseMeasureCollection>(MaxOHx)));
			CHECK_NO_THROW(aDoseStatistic.setMinOCx(::boost::make_shared<algorithms::VolumeToDoseMeasureCollection>(MinOCx)));

			CHECK_EQUAL(aDoseStatistic.getMaximumVoxelPositions(), resultsMaxVoxels);
			CHECK_EQUAL(aDoseStatistic.getMinimumVoxelPositions(), resultsMinVoxels);
			CHECK_EQUAL(aDoseStatistic.getDx() == Dx, true);
			CHECK_EQUAL(aDoseStatistic.getVx() == Vx, true);
			CHECK_EQUAL(aDoseStatistic.getMOHx() == MOHx, true);
			CHECK_EQUAL(aDoseStatistic.getMOCx() == MOCx, true);
			CHECK_EQUAL(aDoseStatistic.getMaxOHx() == MaxOHx, true);
			CHECK_EQUAL(aDoseStatistic.getMinOCx() == MinOCx, true);

			//3) test getters of complex statistics(with stored key and without stored key)
			//getAll*() already tested in (2)
			Vx = algorithms::DoseToVolumeMeasureCollection(algorithms::DoseToVolumeMeasureCollection::Vx, maximum);
			Vx.insertValue(1.1, 1000);
			Vx.insertValue(5.0, 2300);
			Vx.insertValue(90, 90500);
			Vx.insertValue(107, 99000);

			Dx = algorithms::VolumeToDoseMeasureCollection(algorithms::VolumeToDoseMeasureCollection::Dx, volume);
			Dx.insertValue(1000, 1.1);
			Dx.insertValue(2000, 2.0);
			Dx.insertValue(5000, 10.8);
			Dx.insertValue(90000, 89.5);
			Dx.insertValue(98000, 104.4);
			Dx.insertValue(99000, 106.9);

			rttb::algorithms::DoseStatistics aDoseStatisticNewValues(minimum, maximum, mean, stdDeviation,
			        numVoxels, volume);
			aDoseStatisticNewValues.setDx(::boost::make_shared<algorithms::VolumeToDoseMeasureCollection>(Dx));
			aDoseStatisticNewValues.setVx(::boost::make_shared<algorithms::DoseToVolumeMeasureCollection>(Vx));

			CHECK_NO_THROW(aDoseStatisticNewValues.getVx().getValue(1.1));
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx().getValue(90));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValue(1000));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValue(98000));

			CHECK_EQUAL(aDoseStatisticNewValues.getVx().getValue(1.1), Vx.getAllValues().find(1.1)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getVx().getValue(90), Vx.getAllValues().find(90)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getDx().getValue(1000), Dx.getAllValues().find(1000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getDx().getValue(98000), Dx.getAllValues().find(98000)->second);

			//test if key-value combination NOT in map
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getDx().getValue(1001), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getVx().getValue(10), core::DataNotAvailableException);

			double closestDxKey, closestVxKey;
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValue(900, true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValue(99001, true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx().getValue(10, true, closestVxKey));
			CHECK_EQUAL(aDoseStatisticNewValues.getDx().getValue(900, true, closestDxKey), Dx.getAllValues().find(1000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getDx().getValue(99001, true, closestDxKey), Dx.getAllValues().find(99000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getVx().getValue(10, true, closestVxKey), Vx.getAllValues().find(5.0)->second);
			CHECK_EQUAL(closestDxKey, 99000);
			CHECK_EQUAL(closestVxKey, 5);
			
			// relatives only between 0 and 1
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx().getValueRelative(1.1 / aDoseStatistic.getReferenceDose()));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValueRelative(1000 / aDoseStatistic.getVolume()));
			CHECK_THROW(aDoseStatisticNewValues.getVx().getValueRelative(-0.3));
			CHECK_THROW(aDoseStatisticNewValues.getVx().getValueRelative(1.1));
			CHECK_THROW(aDoseStatisticNewValues.getDx().getValueRelative(0.5));

			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValueRelative(900 / aDoseStatistic.getVolume(), true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValueRelative(0.5, true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx().getValueRelative(10 / aDoseStatistic.getReferenceDose(), true, closestVxKey));
			CHECK_EQUAL(aDoseStatisticNewValues.getDx().getValueRelative(900 / aDoseStatistic.getVolume(), true, closestDxKey), Dx.getAllValues().find(1000)->second);
			
			CHECK_EQUAL(aDoseStatisticNewValues.getVx().getValueRelative(10 / aDoseStatistic.getReferenceDose(), true, closestVxKey), Vx.getAllValues().find(5.0)->second);
			CHECK_EQUAL(closestVxKey, 5);

	

			//equal distance to two values. First value is returned.
			CHECK_NO_THROW(aDoseStatisticNewValues.getDx().getValue(1500, true, closestDxKey));
			CHECK_NO_THROW(aDoseStatisticNewValues.getVx().getValue(98.5, true, closestVxKey));
			CHECK_EQUAL(aDoseStatisticNewValues.getDx().getValue(1500, true, closestDxKey), Dx.getAllValues().find(1000)->second);
			CHECK_EQUAL(aDoseStatisticNewValues.getVx().getValue(98.5, true, closestVxKey), Vx.getAllValues().find(90.0)->second);
			CHECK_EQUAL(closestDxKey, 1000);
			CHECK_EQUAL(closestVxKey, 90.0);

			double dummy;
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMinOCx().getValue(25), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMOHx().getValue(9999), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMinOCx().getValue(25, true, dummy),
			                     core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMOHx().getValue(9999, true, dummy),
			                     core::DataNotAvailableException);

			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMinOCx().getValueRelative(25 / aDoseStatistic.getVolume()), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMOHx().getValueRelative(9999 / aDoseStatistic.getVolume()), core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMinOCx().getValueRelative(25 / aDoseStatistic.getVolume(), true, dummy),
				core::DataNotAvailableException);
			CHECK_THROW_EXPLICIT(aDoseStatisticNewValues.getMOHx().getValueRelative(9999 / aDoseStatistic.getVolume(), true, dummy),
				core::DataNotAvailableException);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
