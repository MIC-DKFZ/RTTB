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
			typedef std::map<DoseTypeGy, VolumeType> DoseToVolumeFunctionType;
			typedef std::map<VolumeType, DoseTypeGy> VolumeToDoseFunctionType;

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
			DoseToVolumeFunctionType _Vx;
			VolumeToDoseFunctionType _MOHx;
			VolumeToDoseFunctionType _MOCx;
			VolumeToDoseFunctionType _MaxOHx;
			VolumeToDoseFunctionType _MinOCx;


		public:
			/*! @brief Standard Constructor
			*/
			//DoseStatistics();

			/*! @brief Constructor
				@details the dose statistic values are set. Complex values maximumVoxelLocation, maximumVoxelLocation, Dx, Vx, MOHx, MOCx, MaxOHx and MinOCx are optional
			*/
			DoseStatistics(DoseStatisticType minimum, DoseStatisticType maximum, DoseStatisticType mean,
			               DoseStatisticType stdDeviation, VoxelNumberType numVoxels, VolumeType volume,
			               ResultListPointer minimumVoxelPositions =
			                   NULL,
			               ResultListPointer maximumVoxelPositions =
			                   NULL,
						   VolumeToDoseMeasure _Dx = VolumeToDoseMeasure("Dx", std::map<VolumeType, DoseTypeGy>(), 0.0),
			               DoseToVolumeFunctionType Vx = DoseToVolumeFunctionType(),
			               VolumeToDoseFunctionType MOHx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MOCx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MaxOHx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MinOCx = VolumeToDoseFunctionType(),
			               DoseTypeGy referenceDose = -1);

			~DoseStatistics();


			void setMinimumVoxelPositions(ResultListPointer minimumVoxelPositions);
			void setMaximumVoxelPositions(ResultListPointer maximumVoxelPositions);
			void setDx(const VolumeToDoseMeasure& DxValues);
			void setVx(const VolumeToDoseFunctionType& VxValues);
			void setMOHx(const VolumeToDoseFunctionType& MOHxValues);
			void setMOCx(const VolumeToDoseFunctionType& MOCxValues);
			void setMaxOHx(const VolumeToDoseFunctionType& MaxOHxValues);
			void setMinOCx(const VolumeToDoseFunctionType& MinOCxValues);
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

			/*! @brief Get Vx: the volume irradiated with a dose >= x.
				@return Return absolute volume in absolute cm^3.
				@exception NoDataException if the Vx values have not been set (i.e. the vector is empty)
				@exception NoDataException if the requested Dose is not in the vector
			*/
			VolumeType getVx(DoseTypeGy xDoseAbsolute) const;
			VolumeType getVx(DoseTypeGy xDoseAbsolute, bool findNearestValue,
			                 DoseTypeGy& nearestXDose) const;
			VolumeType getVxRelative(DoseTypeGy xDoseRelative) const;
			VolumeType getVxRelative(DoseTypeGy xDoseRelative, bool findNearestValue,
				DoseTypeGy& nearestXDose) const;
			DoseToVolumeFunctionType getAllVx() const;

			VolumeToDoseMeasure getDx() const;

			/*! @brief Get MOHx: mean dose of the hottest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                   VolumeType& nearestXVolume) const;
			DoseTypeGy getMOHx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy getMOHxRelative(VolumeType xDoseRelative, bool findNearestValue,
				VolumeType& nearestXVolume) const;
			DoseTypeGy getMOHxRelative(VolumeType xDoseRelative) const;
			VolumeToDoseFunctionType getAllMOHx() const;

			/*! @brief Get MOCx: mean dose of the coldest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                   VolumeType& nearestXVolume) const;
			DoseTypeGy getMOCx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy getMOCxRelative(VolumeType xDoseRelative, bool findNearestValue,
				VolumeType& nearestXVolume) const;
			DoseTypeGy getMOCxRelative(VolumeType xDoseRelative) const;
			VolumeToDoseFunctionType getAllMOCx() const;

			/*! @brief Get MaxOHx: Maximum outside of the hottest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMaxOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                     VolumeType& nearestXVolume) const;
			DoseTypeGy getMaxOHx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy getMaxOHxRelative(VolumeType xDoseRelative, bool findNearestValue,
				VolumeType& nearestXVolum) const;
			DoseTypeGy getMaxOHxRelative(VolumeType xDoseRelative) const;
			VolumeToDoseFunctionType getAllMaxOHx() const;

			/*! @brief Get MinOCx: Minimum outside of the coldest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMinOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                     VolumeType& nearestXVolume) const;
			DoseTypeGy getMinOCx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy getMinOCxRelative(VolumeType xDoseRelative, bool findNearestValue,
				VolumeType& nearestXVolume) const;
			DoseTypeGy getMinOCxRelative(VolumeType xDoseRelative) const;
			VolumeToDoseFunctionType getAllMinOCx() const;
		};

	}
}


#endif
