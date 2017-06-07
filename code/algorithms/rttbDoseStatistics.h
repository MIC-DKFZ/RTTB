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

#include "boost/shared_ptr.hpp"

#include "rttbBaseType.h"

#include "RTTBAlgorithmsExports.h"

#include "rttbVolumeToDoseMeasure.h"
#include "rttbDoseToVolumeMeasure.h"

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
			enum complexStatistics { Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx };
			typedef boost::shared_ptr<std::vector<std::pair<DoseTypeGy, VoxelGridID> > > ResultListPointer;
			typedef boost::shared_ptr<DoseStatistics> DoseStatisticsPointer;

		private:
			double getValue(const std::map<double, double>& aMap, double key, bool findNearestValueInstead,
			                double& storedKey) const;

			std::map<double, double>::const_iterator findNearestKeyInMap(const std::map<double, double>& aMap,
			        double key) const;

			DoseStatisticType _maximum;
			DoseStatisticType _minimum;
			ResultListPointer _maximumVoxelPositions;
			ResultListPointer _minimumVoxelPositions;
			DoseStatisticType _mean;
			DoseStatisticType _stdDeviation;
			VoxelNumberType _numVoxels;
			VolumeType _volume;
			DoseTypeGy _referenceDose; //for Vx computation
			VolumeToDoseMeasure _Dx;
			DoseToVolumeMeasure _Vx;
			VolumeToDoseMeasure _MOHx;
			VolumeToDoseMeasure _MOCx;
			VolumeToDoseMeasure _MaxOHx;
			VolumeToDoseMeasure _MinOCx;


		public:
			/*! @brief Standard Constructor
			*/
			//DoseStatistics();

			/*! @brief Constructor
				@details the dose statistic values are set. Complex values maximumVoxelLocation, maximumVoxelLocation, Dx, Vx, MOHx, MOCx, MaxOHx and MinOCx are optional
			*/
			DoseStatistics(DoseStatisticType minimum, DoseStatisticType maximum, DoseStatisticType mean,
			               DoseStatisticType stdDeviation, VoxelNumberType numVoxels, VolumeType volume,
				ResultListPointer minimumVoxelPositions = NULL,
				ResultListPointer maximumVoxelPositions = NULL,
				VolumeToDoseMeasure Dx = VolumeToDoseMeasure(VolumeToDoseMeasure::Dx),
			    DoseToVolumeMeasure Vx = DoseToVolumeMeasure(DoseToVolumeMeasure::Vx),
				VolumeToDoseMeasure MOHx = VolumeToDoseMeasure(VolumeToDoseMeasure::MOHx),
				VolumeToDoseMeasure MOCx = VolumeToDoseMeasure(VolumeToDoseMeasure::MOCx),
				VolumeToDoseMeasure MaxOHx = VolumeToDoseMeasure(VolumeToDoseMeasure::MaxOHx),
				VolumeToDoseMeasure MinOCx = VolumeToDoseMeasure(VolumeToDoseMeasure::MinOCx),
			    DoseTypeGy referenceDose = -1);

			~DoseStatistics();


			void setMinimumVoxelPositions(ResultListPointer minimumVoxelPositions);
			void setMaximumVoxelPositions(ResultListPointer maximumVoxelPositions);
			void setDx(const VolumeToDoseMeasure& DxValues);
			void setVx(const DoseToVolumeMeasure& VxValues);
			void setMOHx(const VolumeToDoseMeasure& MOHxValues);
			void setMOCx(const VolumeToDoseMeasure& MOCxValues);
			void setMaxOHx(const VolumeToDoseMeasure& MaxOHxValues);
			void setMinOCx(const VolumeToDoseMeasure& MinOCxValues);
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

			VolumeToDoseMeasure getDx() const;
			DoseToVolumeMeasure getVx() const;
			VolumeToDoseMeasure getMOHx() const;
			VolumeToDoseMeasure getMOCx() const;
			VolumeToDoseMeasure getMaxOHx() const;
			VolumeToDoseMeasure getMinOCx() const;
		};

	}
}


#endif
