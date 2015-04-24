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
		@brief Data class for storing different statistical values from a RT dose distribution
		@details DoseStatisticsCalculator is used to compute the DoseStatistics
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
			/*! @brief Returns the value of a map belonging to a key
				@details if findNearestValueInstead=true and the key is not in the map, the value belonging to the nearest key is returned. This key is then storedKey.
			*/
			double getValue(const std::map<double, double>& aMap, double key, bool findNearestValueInstead,
			                double& storedKey) const;

			std::map<double, double>::const_iterator findNearestKeyInMap(const std::map<double, double>& aMap, double key) const;

			DoseStatisticType _maximum;
			DoseStatisticType _minimum;
			ResultListPointer _maximumVoxelPositions;
			ResultListPointer _minimumVoxelPositions;
			DoseStatisticType _mean;
			DoseStatisticType _stdDeviation;
			VolumeType _numVoxels;
			VolumeType _volume;
			VolumeToDoseFunctionType _Dx;
			DoseToVolumeFunctionType _Vx;
			VolumeToDoseFunctionType _MOHx;
			VolumeToDoseFunctionType _MOCx;
			VolumeToDoseFunctionType _MaxOHx;
			VolumeToDoseFunctionType _MinOCx;

		public:
			/*! @brief Constructor
				@details the dose statistic values are set. Minimum, maximum, mean, stdDeviation, numVoxels and volume are mandatory. minimumVoxelPositions, maximumVoxelPositions, Dx, Vx, MOHx, MOCx, MaxOHx and MinOCx are optional.
				@param minimum the minimum dose in Gy
				@param maximum the maximum dose in Gy
				@param mean the mean dose in Gy
				@param stdDeviation the stdDeviation of the dose in Gy
				@param numVoxels the amount of voxels (sub-voxel accurate, i.e. not necessarily whole voxels)
				@param volume the volume in cm^3
				@param minimumVoxelPositions a vector containing the function minDose --> VoxelGrid as a pair
				@param maximumVoxelPositions a vector containing the function maxDose --> VoxelGrid as a pair
				@param Dx a map containing the function Volume x --> Dose y with: the minimal dose y is delivered to the volume x.
				@param Vx a map containing the function Dose x --> Volume y with: the volume y is irradiated with a dose >= x.
				@param MOHx a map containing the function Volume x --> Dose y with: the mean dose y of the hottest voxels in volume x.
				@param MOCx a map containing the function Volume x --> Dose y with: the mean dose y of the coldest voxels in volume x.
				@param MaxOHx a map containing the function Volume x --> Dose y with: the maximum dose y outside of the hottest voxels in volume x.
				@param MinOCx a map containing the function Volume x --> Dose y with: the minimum dose y outside of the coldest voxels in volume x.
			*/
			DoseStatistics(DoseStatisticType minimum, DoseStatisticType maximum, DoseStatisticType mean,
			               DoseStatisticType stdDeviation, VolumeType numVoxels, VolumeType volume,
			               ResultListPointer minimumVoxelPositions = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >
			               (std::vector<std::pair<DoseTypeGy, VoxelGridID> >()),
			               ResultListPointer maximumVoxelPositions = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >
			               (std::vector<std::pair<DoseTypeGy, VoxelGridID> >()),
			               VolumeToDoseFunctionType Dx = VolumeToDoseFunctionType(),
			               DoseToVolumeFunctionType Vx = DoseToVolumeFunctionType(),
			               VolumeToDoseFunctionType MOHx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MOCx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MaxOHx = VolumeToDoseFunctionType(),
			               VolumeToDoseFunctionType MinOCx = VolumeToDoseFunctionType());

			~DoseStatistics();

			void setMinimumVoxelPositions(ResultListPointer minimumVoxelPositions);
			void setMaximumVoxelPositions(ResultListPointer maximumVoxelPositions);
			void setDx(const VolumeToDoseFunctionType& DxValues);
			void setVx(const DoseToVolumeFunctionType& VxValues);
			void setMOHx(const VolumeToDoseFunctionType& MOHxValues);
			void setMOCx(const VolumeToDoseFunctionType& MOCxValues);
			void setMaxOHx(const VolumeToDoseFunctionType& MaxOHxValues);
			void setMinOCx(const VolumeToDoseFunctionType& MinOCxValues);

			/*! @brief Get number of voxels in doseIterator, with sub-voxel accuracy.
			*/
			VolumeType getNumberOfVoxels() const;

			VolumeType getVolume() const;

			/*! @brief Get the maximum of the current dose distribution.
				@return the maximum dose in Gy
			*/
			DoseStatisticType getMaximum() const;

			/*! @brief Get a vector of pairs of the the maximum dose VoxelGridIDs together with their dose value in Gy
			*/
			ResultListPointer getMaximumPositions() const;

			/*! @brief Get the minimum of the current dose distribution.
				@return the minimum dose in Gy
			*/
			DoseStatisticType getMinimum() const;

			/*! @brief Get a vector of pairs of the the minimum dose VoxelGridIDs together with their dose value in Gy
			*/
			ResultListPointer getMinimumPositions() const;

			/*! @brief Get the mean of the current dose distribution.
				@return the mean dose in Gy
			*/
			DoseStatisticType getMean() const;

			/*! @brief Get the standard deviation of the current dose distribution.
				@return the standard deviation in Gy
			*/
			DoseStatisticType getStdDeviation() const;

			/*! @brief Get the variance of of the current dose distribution.
				@return the variance in Gy
			*/
			DoseStatisticType getVariance() const;
			/*! @overload
				@details calls getVx(xDoseAbsolute, false, Dummy)
			*/
			VolumeType getVx(DoseTypeGy xDoseAbsolute) const;
			/*! @brief Get Vx: the volume irradiated with a dose >= x.
				@param xDoseAbsolute the dose
				@param findNearestValue if false: only the value (=volume) is returned that corresponds to the exact key (=dose).
										If true: the value corresponding to the nearest key given is returned if the exact value is not in the map
				@param nearestXDose the nearest key (=dose) found
				@return Absolute volume in cm^3.
				@exception NoDataException if the Vx values have not been set (i.e. the vector is empty)
				@exception NoDataException if the requested Dose is not in the precomputed vector
			*/
			VolumeType getVx(DoseTypeGy xDoseAbsolute, bool findNearestValue,
			                 DoseTypeGy& nearestXDose) const;
			/*! @brief Get all computed Dx values
			*/
			DoseToVolumeFunctionType getAllVx() const;

			/*!
			@brief Get Dx: the minimal dose delivered to part x of the current volume.
				@param xVolumeAbsolute the absolute volume in cm^3
				@param findNearestValue if false: only the value (=dose) is returned that corresponds to the exact key (=volume).
										If true: the value corresponding to the nearest key given is returned if the exact value is not in the map
				@param nearestXVolume the nearest key (=volume) found
				@return dose value in Gy.
				@exception NoDataException if the Dx values have not been set (i.e. the vector is empty)
				@exception NoDataException if the requested volume is not in the precomputed vector
			*/
			DoseTypeGy getDx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                 VolumeType& nearestXVolume) const;
			/*! @overload
			@details calls getDx(xVolumeAbsolute, false, Dummy)
			*/
			DoseTypeGy getDx(VolumeType xVolumeAbsolute) const;
			/*! @brief Get all computed Vx values
			*/
			VolumeToDoseFunctionType getAllDx() const;

			/*! @brief Get MOHx: mean dose of the hottest x voxels.
				@return Return dose value in Gy.
				@exception NoDataException if the MOHx values have not been set (i.e. the vector is empty)
				@exception NoDataException if the requested volume is not in the precomputed vector
				@sa getMOHx()
			*/
			DoseTypeGy getMOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                   VolumeType& nearestXVolume) const;
			/*! @overload
				@details calls getMOHx(xVolumeAbsolute, false, Dummy)
			*/
			DoseTypeGy getMOHx(VolumeType xVolumeAbsolute) const;
			/*! @brief Get all computed MOHx values
				@return Return dose value in Gy.
			*/
			VolumeToDoseFunctionType getAllMOHx() const;

			/*! @brief Get MOCx: mean dose of the coldest x voxels.
				@return Return dose value in Gy.
				@exception NoDataException if the MOCx values have not been set (i.e. the vector is empty)
				@exception NoDataException if the requested volume is not in the precomputed vector
				@sa getDx()
			*/
			DoseTypeGy getMOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                   VolumeType& nearestXVolume) const;
			/*! @overload
				@details calls getMOCx(xVolumeAbsolute, false, Dummy)
			*/
			DoseTypeGy getMOCx(VolumeType xVolumeAbsolute) const;
			/*! @brief Get all computed MOCx values
				@return Return dose value in Gy.
			*/
			VolumeToDoseFunctionType getAllMOCx() const;

			/*! @brief Get MaxOHx: Maximum outside of the hottest x voxels.
				@return Return dose value in Gy.
				@exception NoDataException if the MaxOHx values have not been set (i.e. the vector is empty)
				@exception NoDataException if the requested volume is not in the precomputed vector
				@sa getDx()
			*/
			DoseTypeGy getMaxOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                     VolumeType& nearestXVolume) const;
			/*! @overload
				@details calls getMaxOHx(xVolumeAbsolute, false, Dummy)
			*/
			DoseTypeGy getMaxOHx(VolumeType xVolumeAbsolute) const;
			/*! @brief Get all computed MaxOHx values
			*/
			VolumeToDoseFunctionType getAllMaxOHx() const;

			/*! @brief Get MinOCx: Minimum outside of the coldest x voxels.
				@return Return dose value in Gy.
				@exception NoDataException if the MinOCx values have not been set (i.e. the vector is empty)
				@exception NoDataException if the requested volume is not in the precomputed vector
				@sa getDx()
			*/
			DoseTypeGy getMinOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
			                     VolumeType& nearestXVolume) const;
			/*! @overload
				@details calls getMinOCx(xVolumeAbsolute, false, Dummy)
			*/
			DoseTypeGy getMinOCx(VolumeType xVolumeAbsolute) const;
			/*! @brief Get all computed MinOCx values
			*/
			VolumeToDoseFunctionType getAllMinOCx() const;
		};

	}
}


#endif
