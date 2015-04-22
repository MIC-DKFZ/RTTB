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

#include <boost/make_shared.hpp>
#include <vector>
#include <map>

#include "rttbBaseType.h"
#include "rttbDoseIteratorInterface.h"

namespace rttb
{

	namespace algorithms
	{

		/*! @class DoseStatistics
		@brief This is a data class storing different statistical values from a rt dose distribution
		@sa DoseStatisticsCalculator
		*/
		class DoseStatistics
		{
		public:
			enum complexStatistics { Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx};
			typedef boost::shared_ptr<std::vector<std::pair<DoseTypeGy, VoxelGridID> > > ResultListPointer;
			typedef boost::shared_ptr<DoseStatistics> DoseStatisticsPointer;
			typedef std::map<DoseTypeGy, VolumeType> DoseToVolumeFunctionType;
			typedef std::map<VolumeType, DoseTypeGy> VolumeToDoseFunctionType;

		private:
			double getValue(std::map<double, double> aMap, double key, bool findNearestValueInstead,
			                double& storedKey) const;

			std::map<double, double>::const_iterator findNearestKeyInMap(std::map<double, double> aMap, double key) const;

			DoseStatisticType _maximum;
			DoseStatisticType _minimum;
			ResultListPointer _maximumVoxelPositions;
			ResultListPointer _minimumVoxelPositions;
			DoseStatisticType _mean;
			DoseStatisticType _stdDeviation;
			unsigned int _numVoxels;
			VolumeType _volume;
			VolumeToDoseFunctionType _Dx;
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
				@detail the dose statistic values are set. Complex values maximumVoxelLocation, maximumVoxelLocation, Dx, Vx, MOHx, MOCx, MaxOHx and MinOCx are optional
			*/
			DoseStatistics(DoseStatisticType minimum, DoseStatisticType maximum, DoseStatisticType mean,
			               DoseStatisticType stdDeviation, unsigned int numVoxels, VolumeType volume,
			               ResultListPointer minimumVoxelPositions = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >
			               (std::vector<std::pair<DoseTypeGy, VoxelGridID> >()),
			               ResultListPointer maximumVoxelPositions = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >
			               (std::vector<std::pair<DoseTypeGy, VoxelGridID> >()),
			               DoseToVolumeFunctionType Dx = DoseToVolumeFunctionType(),
			               VolumeToDoseFunctionType Vx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MOHx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MOCx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MaxOHx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MinOCx = VolumeToDoseFunctionType());

			~DoseStatistics();

			void setMinimumVoxelPositions(ResultListPointer minimumVoxelPositions);
			void setMaximumVoxelPositions(ResultListPointer maximumVoxelPositions);
			void setDx(const DoseToVolumeFunctionType& DxValues);
			void setVx(const VolumeToDoseFunctionType& VxValues);
			void setMOHx(const VolumeToDoseFunctionType& MOHxValues);
			void setMOCx(const VolumeToDoseFunctionType& MOCxValues);
			void setMaxOHx(const VolumeToDoseFunctionType& MaxOHxValues);
			void setMinOCx(const VolumeToDoseFunctionType& MinOCxValues);

			/*! @brief Get number of voxels in doseIterator, with sub-voxel accuracy.
			*/
			unsigned int getNumberOfVoxels() const;

			VolumeType getVolume() const;

			/*! @brief Get the maximum of the current dose distribution.
				@return Return the maximum dose in Gy
			*/
			DoseStatisticType getMaximum() const;

			/*! @brief Get a vector of the the maximum dose VoxelGridIDs together with their dose value in Gy
				@exception InvalidDoseException if the vector has not been set (i.e. is empty)
			*/
			ResultListPointer getMaximumPositions() const;

			/*! @brief Get the minimum of the current dose distribution.
				@return Return the minimum dose in Gy
			*/
			DoseStatisticType getMinimum() const;

			/*! @brief Get a vector of the the minimum dose VoxelGridIDs together with their dose value in Gy
			@exception InvalidDoseException if the vector has not been set (i.e. is empty)
			*/
			ResultListPointer getMinimumPositions() const;

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
			DoseToVolumeFunctionType getAllVx() const;

			/*! @brief Get Dx: the minimal dose delivered to part x of the current volume.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the Dx values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getDx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                 VolumeType& nearestXVolume) const;
			DoseTypeGy getDx(VolumeType xVolumeAbsolute) const;
			VolumeToDoseFunctionType getAllDx() const;

			/*! @brief Get MOHx: mean dose of the hottest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                   VolumeType& nearestXVolume) const;
			DoseTypeGy getMOHx(VolumeType xVolumeAbsolute) const;
			VolumeToDoseFunctionType getAllMOHx() const;

			/*! @brief Get MOCx: mean dose of the coldest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                   VolumeType& nearestXVolume) const;
			DoseTypeGy getMOCx(VolumeType xVolumeAbsolute) const;
			VolumeToDoseFunctionType getAllMOCx() const;

			/*! @brief Get MaxOHx: Maximum outside of the hottest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMaxOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                     VolumeType& nearestXVolume) const;
			DoseTypeGy getMaxOHx(VolumeType xVolumeAbsolute) const;
			VolumeToDoseFunctionType getAllMaxOHx() const;

			/*! @brief Get MinOCx: Minimum outside of the coldest x voxels.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the values have not been set (i.e. the vector is empty)
			*/
			DoseTypeGy getMinOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                     VolumeType& nearestXVolume) const;
			DoseTypeGy getMinOCx(VolumeType xVolumeAbsolute) const;
			VolumeToDoseFunctionType getAllMinOCx() const;
		};

	}
}


#endif
