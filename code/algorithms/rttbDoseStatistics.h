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

#include "rttbBaseType.h"
#include "rttbDoseIteratorInterface.h"

namespace rttb{

	namespace algorithms{

		/*! @class DoseStatistics
		@brief This is a class calculating different statistical values from a rt dose distribution
		These values range fram standard statistical values such as minimum, maximum and mean to more 
		dose specific properties such as Vx (volume iradiated with a dose >=x), Dx (minimal dose delivered 
		to x% of the VOI, and MOHx (mean in the hottest volume).
		*/
		class DoseStatistics
			{
			public:
				typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
				typedef boost::shared_ptr<std::vector<std::pair<DoseTypeGy,VoxelGridID> > > ResultListPointer;

			private: 
				DoseIteratorPointer _doseIterator;
				DoseStatisticType _maximum;
				DoseStatisticType _minimum;
				DoseStatisticType _mean;
				DoseStatisticType _stdDeviation;
				DoseStatisticType _variance;
				DoseStatisticType _numVoxels;

				/* Contains relevant dose values sorted in descending order.
				*/
				std::vector<DoseTypeGy> doseVector;

				/*! Contains the corresponding voxel proprtions to the values in doseVector. 
				*/
				std::vector<double> voxelProportionVector;

				bool initSuccess;

				/*! @brief Calculation of basic dose statistics. It will be called in Constructor.
					@exception NullPointerException Thrown if _doseIterator is NULL.
				*/
				bool init();



			public:
				/*! @brief Standard Constructor
				*/
				DoseStatistics();

				/*! @brief Constructor
				*/
				DoseStatistics(DoseIteratorPointer aDoseIterator);

				/*! @brief Set new dose data for statistics. Statistics will be re-initialized.
				*/
				void setDoseIterator(DoseIteratorPointer aDoseIterator);

				/*! @brief Get dose iterator.
				*/
				DoseIteratorPointer getDoseIterator() const;


				/*! @brief Get number of voxels in doseIterator, with sub-voxel accuracy.
        @exception InvalidDoseException Thrown if statistic was not initialized.
				*/
				DoseStatisticType getNumberOfVoxels();

				/*! @brief Get the maximum of the current dose distribution.
					@param maxVoxelVector: vector of all voxel with the dose=maximum.
					@return Return the maximum dose in Gy, or -1 if an error occured.
          @exception InvalidDoseException Thrown if statistic was not initialized.
				*/
				DoseStatisticType getMaximum( ResultListPointer maxVoxelVector) const;

				/*! @brief Get the minimum of the current dose distribution.
					@param minVoxelVector: vector of all voxel with the dose=minimum.
					@param number: the number of dose voxels with dose=minimum that are stored in minVoxelVector.
					Only the first occurences are stored. The default value is 100.
					@return Return the minimum dose in Gy, or -1 if an error occured.
          @exception InvalidDoseException Thrown if statistic was not initialized.
				*/
				DoseStatisticType getMinimum( ResultListPointer minVoxelVector, int number=100) const;

				/*! @brief Get the mean of the current dose distribution.
					@return Return the mean dose in Gy, or -1 if an error occured.
          @exception InvalidDoseException Thrown if statistic was not initialized.
				*/
				DoseStatisticType getMean() const;

				/*! @brief Get the standard deviation of the current dose distribution.
					@return Return the standard deviation in Gy, or -1 if an error occured.
          @exception InvalidDoseException Thrown if statistic was not initialized.
				*/
				DoseStatisticType getStdDeviation() const;

				/*! @brief Get the variance of of the current dose distribution.
					@return Return the variance in Gy, or -1 if an error occured.
          @exception InvalidDoseException Thrown if statistic was not initialized.
				*/
				DoseStatisticType getVariance() const;

				/*! @brief Get Vx: the volume irradiated with a dose >= x.
					@return Return absolute volume in absolute cm3.
				*/
				VolumeType getVx(DoseTypeGy xDoseAbsolute) const;

				/*! @brief Get Dx: the minimal dose delivered to part x of the current volume.
					@return Return dose value in Gy.
				*/	
				DoseTypeGy getDx(VolumeType xVolumeAbsolute) const;

				/*! @brief Get MOHx: mean dose of the hottest x voxels.
					@return Return dose value in Gy.
				*/	
				DoseTypeGy getMOHx(VolumeType xVolumeAbsolute) const;

				/*! @brief Get MOCx: mean dose of the coldest x voxels.
					@return Return dose value in Gy.
				*/
				DoseTypeGy getMOCx(VolumeType xVolumeAbsolute) const;

				/*! @brief Get MaxOHx: Maximum outside of the hottest x voxels.
					@return Return dose value in Gy.
				*/
				DoseTypeGy getMaxOHx(VolumeType xVolumeAbsolute) const;

				/*! @brief Get MinOCx: Minimum outside of the coldest x voxels.
					@return Return ose value in Gy.
				*/
				DoseTypeGy getMinOCx(VolumeType xVolumeAbsolute) const;
			};

		}
	}


#endif
