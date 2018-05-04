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

#ifndef __DOSE_STATISTICS_H
#define __DOSE_STATISTICS_H

#include <vector>
#include <map>

#include <rttbCommon.h>

#include <boost/make_shared.hpp>

#include "rttbBaseType.h"

#include "RTTBAlgorithmsExports.h"

#include "rttbVolumeToDoseMeasureCollection.h"
#include "rttbDoseToVolumeMeasureCollection.h"

namespace rttb
{

	namespace algorithms
	{

		/*! @class DoseStatistics
		@brief This is a data class storing different statistical values from a rt dose distribution
		@sa DoseStatisticsCalculator
		*/
		class RTTBAlgorithms_EXPORT DoseStatistics
		{
		public:
      rttbClassMacroNoParent(DoseStatistics);
			typedef boost::shared_ptr<std::vector<std::pair<DoseTypeGy, VoxelGridID> > > ResultListPointer;
			using VolumeToDoseMeasureCollectionPointer = boost::shared_ptr<VolumeToDoseMeasureCollection>;
			using DoseToVolumeMeasureCollectionPointer = boost::shared_ptr<DoseToVolumeMeasureCollection>;

		private:
			double getValue(const std::map<double, double>& aMap, double key, bool findNearestValueInstead,
			                double& storedKey) const;

			std::map<double, double>::const_iterator findNearestKeyInMap(const std::map<double, double>& aMap,
			        double key) const;

      DoseStatisticType _minimum;
			DoseStatisticType _maximum;
      DoseStatisticType _mean;
      DoseStatisticType _stdDeviation;
      VoxelNumberType _numVoxels;
      VolumeType _volume;
      ResultListPointer _minimumVoxelPositions;
			ResultListPointer _maximumVoxelPositions;
			VolumeToDoseMeasureCollectionPointer _Dx;
			DoseToVolumeMeasureCollectionPointer _Vx;
			VolumeToDoseMeasureCollectionPointer _MOHx;
			VolumeToDoseMeasureCollectionPointer _MOCx;
			VolumeToDoseMeasureCollectionPointer _MaxOHx;
			VolumeToDoseMeasureCollectionPointer _MinOCx;
      DoseTypeGy _referenceDose; //for Vx computation

		public:
			/*! @brief Standard Constructor
			*/
			//DoseStatistics();

			/*! @brief Constructor
				@details the dose statistic values are set. Complex values maximumVoxelLocation, maximumVoxelLocation, Dx, Vx, MOHx, MOCx, MaxOHx and MinOCx are optional
			*/
			DoseStatistics(DoseStatisticType minimum, DoseStatisticType maximum, DoseStatisticType mean,
			               DoseStatisticType stdDeviation, VoxelNumberType numVoxels, VolumeType volume,
				ResultListPointer minimumVoxelPositions = nullptr,
				ResultListPointer maximumVoxelPositions = nullptr,
				VolumeToDoseMeasureCollection Dx = VolumeToDoseMeasureCollection(VolumeToDoseMeasureCollection::Dx),
			    DoseToVolumeMeasureCollection Vx = DoseToVolumeMeasureCollection(DoseToVolumeMeasureCollection::Vx),
				VolumeToDoseMeasureCollection MOHx = VolumeToDoseMeasureCollection(VolumeToDoseMeasureCollection::MOHx),
				VolumeToDoseMeasureCollection MOCx = VolumeToDoseMeasureCollection(VolumeToDoseMeasureCollection::MOCx),
				VolumeToDoseMeasureCollection MaxOHx = VolumeToDoseMeasureCollection(VolumeToDoseMeasureCollection::MaxOHx),
				VolumeToDoseMeasureCollection MinOCx = VolumeToDoseMeasureCollection(VolumeToDoseMeasureCollection::MinOCx),
			    DoseTypeGy referenceDose = -1);

			~DoseStatistics();


			void setMinimumVoxelPositions(ResultListPointer minimumVoxelPositions);
			void setMaximumVoxelPositions(ResultListPointer maximumVoxelPositions);
			void setDx(VolumeToDoseMeasureCollectionPointer DxValues);
			void setVx(DoseToVolumeMeasureCollectionPointer VxValues);
			void setMOHx(VolumeToDoseMeasureCollectionPointer MOHxValues);
			void setMOCx(VolumeToDoseMeasureCollectionPointer MOCxValues);
			void setMaxOHx(VolumeToDoseMeasureCollectionPointer MaxOHxValues);
			void setMinOCx(VolumeToDoseMeasureCollectionPointer MinOCxValues);
			void setReferenceDose(DoseTypeGy referenceDose);

			/*! @brief Get number of voxels in doseIterator, with sub-voxel accuracy.
			*/
			VoxelNumberType getNumberOfVoxels() const;

			/*! @brief Get the volume of the voxels in doseIterator (in cm3), with sub-voxel accuracy
			*/
			VolumeType getVolume() const;

			/*! @brief Get the reference dose for Vx computation
			*/
			DoseTypeGy getReferenceDose() const;

			/*! @brief Get the maximum of the current dose distribution.
				@return Return the maximum dose in Gy
			*/
			DoseStatisticType getMaximum() const;

			/*! @brief Get a vector of the the maximum dose VoxelGridIDs together with their dose value in Gy
				@exception InvalidDoseException if the vector has not been set (i.e. is empty)
			*/
			ResultListPointer getMaximumVoxelPositions() const;

			/*! @brief Get the minimum of the current dose distribution.
				@return Return the minimum dose in Gy
			*/
			DoseStatisticType getMinimum() const;

			/*! @brief Get a vector of the the minimum dose VoxelGridIDs together with their dose value in Gy
			@exception InvalidDoseException if the vector has not been set (i.e. is empty)
			*/
			ResultListPointer getMinimumVoxelPositions() const;

			/*! @brief Get the mean of the current dose distribution.
				@return Return the mean dose in Gy
			*/
			DoseStatisticType getMean() const;

			/*! @brief Get the standard deviation of the current dose distribution.
				@return Return the standard deviation in Gy
			*/
			DoseStatisticType getStdDeviation() const;

			/*! @brief Get the variance of of the current dose distribution.
				@return Return the variance in Gy
			*/
			DoseStatisticType getVariance() const;

			VolumeToDoseMeasureCollection getDx() const;
			DoseToVolumeMeasureCollection getVx() const;
			VolumeToDoseMeasureCollection getMOHx() const;
			VolumeToDoseMeasureCollection getMOCx() const;
			VolumeToDoseMeasureCollection getMaxOHx() const;
			VolumeToDoseMeasureCollection getMinOCx() const;
		};

	}
}


#endif
