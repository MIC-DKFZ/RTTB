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
		@brief Class for calculating different statistical values from a RT dose distribution
		@details These values range from standard statistical values such as minimum, maximum and mean to more
		complex dose specific measures such as Vx (volume irradiated with a dose >=x), Dx (minimal dose delivered
		to x% of the VOI) or MOHx (mean in the hottest volume). For a complete list, see calculateDoseStatistics().
		@note the complex dose statistics are precomputed and cannot be computed "on the fly" lateron! The doses/volumes that should be used for precomputation have to be set in calculateDoseStatistics()
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

			/*! @brief Contains relevant dose values sorted in descending order.
			*/
			std::vector<DoseTypeGy> _doseVector;

			/*! @brief Contains the corresponding voxel proportions to the values in doseVector.
			*/
			std::vector<double> _voxelProportionVector;
			/*! @brief The doseStatistics are stored here.
			*/
			DoseStatisticsPointer _statistics;

			bool _simpleDoseStatisticsCalculated;

			/*! @brief Calculates the positions where the dose has its maximum
				@param maxNumberMaximaPositions the maximal amount of computed positions
				@pre maximumDose must be defined in _statistics with the correct value
			*/
			ResultListPointer computeMaximumPositions(unsigned int maxNumberMaximaPositions) const;
			/*! @brief Calculates the positions where the dose has its minimum
				@param maxNumberMinimaPositions the maximal amount of computed positions (they are read sequentially using the iterator until maxNumberMinimaPositions have been read, other positions are not considered)
				@pre minimumDose must be defined in _statistics with the correct value
			*/
			ResultListPointer computeMinimumPositions(unsigned int maxNumberMinimaPositions) const;

			VolumeType computeVx(DoseTypeGy xDoseAbsolute) const;
			DoseTypeGy computeDx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy computeMOHx(VolumeType xVolumeAbsolute) const;
			DoseTypeGy computeMOCx(DoseTypeGy xVolumeAbsolute) const;
			DoseTypeGy computeMaxOHx(DoseTypeGy xVolumeAbsolute) const;
			DoseTypeGy computeMinOCx(DoseTypeGy xVolumeAbsolute) const;

			DoseToVolumeFunctionType computeDoseToVolumeFunctionMulti(const std::vector<double>& precomputeDoseValues,
			        DoseStatistics::complexStatistics name) const;
			VolumeToDoseFunctionType computeVolumeToDoseFunctionMulti(const std::vector<double>& precomputeVolumeValues,
			        DoseStatistics::complexStatistics name) const;

			/*! @brief Calculates simple dose statistics (min, mean, max, stdDev, minDosePositions, maxDosePositions)
				@param maxNumberMinimaPositions the maximal amount of computed positions where the dose has its minimum that is computed
				@param maxNumberMaximaPositions the maximal amount of computed positions where the dose has its maximum that is computed
			*/
			void calculateSimpleDoseStatistics(unsigned int maxNumberMinimaPositions, unsigned int maxNumberMaximaPositions);
			/*! @brief Calculates complex dose statistics (Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx)
				@warning computations can take quite long (>1 min) for large structures as many statistics are precomputed
			*/
			void calculateComplexDoseStatistics(const std::vector<double>& precomputeDoseValues,
			                                    const std::vector<double>& precomputeVolumeValues);


		public:
			~DoseStatisticsCalculator();

			/*! @brief Constructor
				@param aDoseIterator the dose to be analyzed
			*/
			DoseStatisticsCalculator(DoseIteratorPointer aDoseIterator);

			DoseIteratorPointer getDoseIterator() const;

			/*! @brief Calculatues dose statistics
				@details The following statistics are calculated always (i.e. also if computeComplexMeasures=false):
						<ul>
							<li>minimum dose
							<li>mean dose
							<li>maximum dose
							<li>standard deviation dose
							<li>voxel positions of minimum dose
							<li>voxel positions of maximum dose
						</ul>
						Additionally, these statistics are computed if computeComplexMeasures=true:
						<ul>
							<li>Dx (the minimal dose delivered to part x of the current volume)
							<li>Vx (the volume irradiated with a dose >= x)
							<li>MOHx (mean dose of the hottest x voxels)
							<li>MOCx (mean dose of the coldest x voxels)
							<li>MaxOHx (Maximum outside of the hottest x voxels)
							<li>MinOCx (Minimum outside of the coldest x voxels)
						</ul>
						Default values for precomputeDoseValues are 0.02, 0.05, 0.1, 0.9, 0.95 and 0.98 with respect to maxDose.
						Default values for precomputeVolumeValues are 0.02, 0.05, 0.1, 0.9, 0.95 and 0.98 with respect to volume.
				@param computeComplexMeasures should complex statistics be calculated?
				@param precomputeDoseValues the dose values for Vx precomputation
				@param precomputeVolumeValues the volume values for Dx, MOHx, MOCx, MaxOHx and MinOCx precomputation
				@param maxNumberMinimaPositions the maximal amount of computed positions where the dose has its minimum that is computed
				@param maxNumberMaximaPositions the maximal amount of computed positions where the dose has its maximum that is computed
				@warning if computeComplexMeasures==true, computations can take quite long (>1 min) for large structures as many statistics are precomputed
				@note the complex dose statistics are precomputed and cannot be computed "on the fly" lateron! The doses/volumes that should be used for precomputation have to be set by in precomputeDoseValues and precomputeVolumeValues. Only these values can be requested in DoseStatistics!
			*/
			DoseStatisticsPointer calculateDoseStatistics(bool computeComplexMeasures = false,
			        const std::vector<double>& precomputeDoseValues = std::vector<double>(),
			        const std::vector<double>& precomputeVolumeValues = std::vector<double>(), unsigned int maxNumberMinimaPositions = 100,
			        unsigned int maxNumberMaximaPositions = 100);

		};

	}
}


#endif
