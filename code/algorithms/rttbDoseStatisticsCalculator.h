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
// @version $Revision: 707 $ (last changed revision)
// @date    $Date: 2014-09-04 16:37:24 +0200 (Do, 04 Sep 2014) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#ifndef __DOSE_STATISTICS_CALCULATOR_H
#define __DOSE_STATISTICS_CALCULATOR_H

#include <vector>

#include "rttbBaseType.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbDoseStatistics.h"

namespace rttb
{

	namespace algorithms
	{

		/*! @class DoseStatisticsCalculator
		@brief This is a class calculating different statistical values from a rt dose distribution
		These values range from standard statistical values such as minimum, maximum and mean to more
		dose specific properties such as Vx (volume irradiated with a dose >=x), Dx (minimal dose delivered
		to x% of the VOI, and MOHx (mean in the hottest volume).
		*/
		class DoseStatisticsCalculator
		{
		public:
			typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
			typedef DoseStatistics::ResultListPointer ResultListPointer;
			typedef DoseStatistics::DoseStatisticsPointer DoseStatisticsPointer;
			typedef DoseStatistics::DoseToVolumeFunctionType DoseToVolumeFunctionType;
			typedef DoseStatistics::VolumeToDoseFunctionType VolumeToDoseFunctionType;

		private:
			DoseIteratorPointer _doseIterator;

			/* Contains relevant dose values sorted in descending order.
			*/
			std::vector<DoseTypeGy> _doseVector;

			/*! Contains the corresponding voxel proportions to the values in doseVector.
			*/
			std::vector<double> _voxelProportionVector;


			DoseStatisticsPointer _statistics;

			bool simpleDoseStatisticsCalculated;

			/*! @brief Calculation of basic dose statistics. It will be called in Constructor.
				@exception NullPointerException Thrown if _doseIterator is NULL.
			*/


			ResultListPointer computeMaximumPositions(unsigned int maxNumberMaxima);

			ResultListPointer computeMinimumPositions(unsigned int maxNumberMinima);
			VolumeType computeVx(DoseTypeGy xDoseAbsolute) const;
			DoseTypeGy computeDx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy computeMOHx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy computeMOCx(DoseTypeGy xVolumeAbsolute) const;
			DoseTypeGy computeMaxOHx(DoseTypeGy xVolumeAbsolute) const;
			DoseTypeGy computeMinOCx(DoseTypeGy xVolumeAbsolute) const;

			DoseToVolumeFunctionType computeDoseToVolumeMulti(const std::vector<double>& precomputeDoseValues,
			        DoseStatistics::complexStatistics name) const;
			VolumeToDoseFunctionType computeVolumeToDoseFunctionMulti(const std::vector<double>& precomputeVolumeValues,
			        DoseStatistics::complexStatistics name) const;

			void calculateSimpleDoseStatistics();
			void calculateComplexDoseStatistics(const std::vector<double>& precomputeDoseValues,
			                                    const std::vector<double>& precomputeVolumeValues);


		public:
			/*! @brief Standard Constructor
			*/
			DoseStatisticsCalculator();

			~DoseStatisticsCalculator();

			/*! @brief Constructor
			*/
			DoseStatisticsCalculator(DoseIteratorPointer aDoseIterator);

			/*! @brief Set new dose data for statistics. Statistics will be re-initialized.
			*/
			void setDoseIterator(DoseIteratorPointer aDoseIterator);

			DoseIteratorPointer getDoseIterator() const;

			DoseStatisticsPointer calculateDoseStatistics(bool computeComplexMeasures = false,
			        const std::vector<double>& precomputeDoseValues = std::vector<double>(),
			        const std::vector<double>& precomputeVolumeValues = std::vector<double>());

		};

	}
}


#endif
