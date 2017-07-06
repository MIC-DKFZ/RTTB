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

#ifndef __DOSE_STATISTICS_CALCULATOR_H
#define __DOSE_STATISTICS_CALCULATOR_H

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "rttbDoseIteratorInterface.h"
#include "rttbDoseStatistics.h"

#include "RTTBAlgorithmsExports.h"

#include "rttbDxVolumeToDoseMeasureCollectionCalculator.h"
#include "rttbVxDoseToVolumeMeasureCollectionCalculator.h"
#include "rttbMOHxVolumeToDoseMeasureCollectionCalculator.h"
#include "rttbMOCxVolumeToDoseMeasureCollectionCalculator.h"
#include "rttbMaxOHxVolumeToDoseMeasureCollectionCalculator.h"
#include "rttbMinOCxVolumeToDoseMeasureCollectionCalculator.h"

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
		class RTTBAlgorithms_EXPORT DoseStatisticsCalculator
		{
		public:
			typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
			typedef DoseStatistics::ResultListPointer ResultListPointer;
			typedef DoseStatistics::DoseStatisticsPointer DoseStatisticsPointer;

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
			bool _complexDoseStatisticsCalculated;

			bool _multiThreading;

			::boost::shared_ptr<boost::shared_mutex> _mutex;

			::boost::shared_ptr<VxDoseToVolumeMeasureCollectionCalculator> _Vx;
			::boost::shared_ptr<DxVolumeToDoseMeasureCollectionCalculator> _Dx;
			::boost::shared_ptr<MOHxVolumeToDoseMeasureCollectionCalculator> _MOHx;
			::boost::shared_ptr<MOCxVolumeToDoseMeasureCollectionCalculator> _MOCx;
			::boost::shared_ptr<MaxOHxVolumeToDoseMeasureCollectionCalculator> _MaxOHx;
			::boost::shared_ptr<MinOCxVolumeToDoseMeasureCollectionCalculator> _MinOCx;

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
			
			/*! @brief Calculates simple dose statistics (min, mean, max, stdDev, minDosePositions, maxDosePositions)
				@param maxNumberMinimaPositions the maximal amount of computed positions where the dose has its minimum that is computed
				@param maxNumberMaximaPositions the maximal amount of computed positions where the dose has its maximum that is computed
			*/
			void calculateSimpleDoseStatistics(unsigned int maxNumberMinimaPositions,
			                                   unsigned int maxNumberMaximaPositions);
			/*! @brief Calculates complex dose statistics (Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx)
				@warning computations can take quite long (>1 min) for large structures as many statistics are precomputed
			*/
			void calculateComplexDoseStatistics(DoseTypeGy referenceDose,
			                                    const std::vector<double>& precomputeDoseValues,
			                                    const std::vector<double>& precomputeVolumeValues);


		public:
			~DoseStatisticsCalculator();

			/*! @brief Constructor
				@param aDoseIterator the dose to be analyzed
			*/
			DoseStatisticsCalculator(DoseIteratorPointer aDoseIterator);

			DoseIteratorPointer getDoseIterator() const;

			/*! @brief Compute simple or complex dose statistics with default relative x values and the maximum dose as default reference dose (for Vx computation)
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
			<li>Dx (the minimal dose delivered to a volume >= x)
			<li>Vx (the volume irradiated with a dose >= x)
			<li>MOHx (mean dose of the hottest x volume)
			<li>MOCx (mean dose of the coldest x volume)
			<li>MaxOHx (Maximum outside of the hottest x volume)
			<li>MinOCx (Minimum outside of the coldest x volume)
			</ul>
			Default x values for Vx are 0.02, 0.05, 0.1, 0.9, 0.95 and 0.98, with respect to maxDose.
			Default x values for Dx, MOHx, MOCx, MaxOHx and MinOCx are 0.02, 0.05, 0.1, 0.9, 0.95 and 0.98, with respect to volume.
			@param computeComplexMeasures should complex statistics be calculated? If it is true, the complex dose statistics will be calculated with default relative x values and the maximum dose as reference dose
			@param maxNumberMinimaPositions the maximal amount of computed positions where the dose has its minimum that is computed
			@param maxNumberMaximaPositions the maximal amount of computed positions where the dose has its maximum that is computed
			@warning If computeComplexMeasures==true, computations can take quite long (>1 min) for large structures as many statistics are precomputed
			@note The complex dose statistics are precomputed and cannot be computed "on the fly" lateron! Only the default x values can be requested in DoseStatistics!
			*/
			DoseStatisticsPointer calculateDoseStatistics(bool computeComplexMeasures = false,
			        unsigned int maxNumberMinimaPositions = 10,
			        unsigned int maxNumberMaximaPositions = 10);

			/*! @brief Compute complex dose statistics with given reference dose and default relative x values
			@param referenceDose the reference dose to compute Vx, normally it should be the prescribed dose
			@param maxNumberMinimaPositions the maximal amount of computed positions where the dose has its minimum that is computed
			@param maxNumberMaximaPositions the maximal amount of computed positions where the dose has its maximum that is computed
			@exception InvalidParameterException thrown if referenceDose <= 0
			@warning Computations can take quite long (>1 min) for large structures as many statistics are precomputed
			@note The complex dose statistics are precomputed and cannot be computed "on the fly" lateron! Only the default x values can be requested in DoseStatistics!
			*/
			DoseStatisticsPointer calculateDoseStatistics(DoseTypeGy referenceDose,
			        unsigned int maxNumberMinimaPositions = 10,
			        unsigned int maxNumberMaximaPositions = 10);

			/*! @brief Compute complex dose statistics with given relative x values and reference dose
			@param precomputeDoseValues the relative dose values for Vx precomputation, e.g. 0.02, 0.05, 0.95...
			@param precomputeVolumeValues the relative volume values for Dx, MOHx, MOCx, MaxOHx and MinOCx precomputation, e.g. 0.02, 0.05, 0.95...
			@param referenceDose the reference dose to compute Vx, normally it should be the prescribed dose. Default value is the maximum dose.
			@param maxNumberMinimaPositions the maximal amount of computed positions where the dose has its minimum that is computed
			@param maxNumberMaximaPositions the maximal amount of computed positions where the dose has its maximum that is computed
			@warning Computations can take quite long (>1 min) for large structures as many statistics are precomputed
			@note The complex dose statistics are precomputed and cannot be computed "on the fly" lateron! The doses/volumes that should be used for precomputation have to be set by in precomputeDoseValues and precomputeVolumeValues. Only these values can be requested in DoseStatistics!
			*/
			DoseStatisticsPointer calculateDoseStatistics(const std::vector<double>& precomputeDoseValues,
			        const std::vector<double>& precomputeVolumeValues, DoseTypeGy referenceDose = -1,
			        unsigned int maxNumberMinimaPositions = 10,
			        unsigned int maxNumberMaximaPositions = 10);

			/*! @brief Adds additonal precompute values for all complex Dose Statistics
				@exception InvalidDoseException if complexDoseStatistics are not already calculated
			*/
			void addPrecomputeValues(const std::vector<double>& values);

			/*! @brief Recalculates the DoseMeasures for all complex Dose Statistics
				@exception InvalidDoseException if complexDoseStatistics are not already calculated
			*/
			void recalculateDoseStatistics();

			void setMultiThreading(bool choice);
		};

	}
}


#endif
