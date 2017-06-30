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
// @version $Revision: 1674 $ (last changed revision)
// @date    $Date: 2017-01-27 10:34:46 +0100 (Fr, 27 Jan 2017) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#ifndef __DOSE_TO_VOLUME_MEASURE_COLLECTION_CALCULATOR_H
#define __DOSE_TO_VOLUME_MEASURE_COLLECTION_CALCULATOR_H

#include <vector>
#include <map>

#include "rttbBaseType.h"

#include "RTTBAlgorithmsExports.h"
#include "rttbDoseToVolumeMeasureCollection.h"

#include "rttbDoseIteratorInterface.h"

#include <boost/shared_ptr.hpp>
#include "rttbDoseStatistics.h"

namespace rttb
{

	namespace algorithms
	{
		/*! @class DoseToVolumeMeasureCollectionCalculator
		@brief Class for calculating DoseToVolume measures
		@details This class is used as a basis for DoseToVolumeMeasureCollectionCalculators. It implements the compute() method 
		that is the same for every inheriting subclass, it accesses computeSpecificValue, which has to be implemented
		for each different complex statistic.
		*/
		class RTTBAlgorithms_EXPORT DoseToVolumeMeasureCollectionCalculator {

		public:
			typedef std::map<DoseTypeGy, VolumeType> VolumeToDoseFunctionType;

		protected:
			core::DoseIteratorInterface::DoseIteratorPointer _doseIterator;

		private:
			DoseTypeGy _referenceDose;
			DoseStatistics::DoseToVolumeMeasureCollectionPointer _measureCollection;
			std::vector<double> _precomputeDoseValues;
			bool _multiThreading;

		public:
			/*! @brief Computes not already computed values for the measureCollection. Algorithm for the specific complex Statistic has to be implemented in the corresponding subclass.
			*/
			void compute();
			/*! @brief Adds additional values to the _precomputeDoseValues vector.
				@exception InvalidParameterException If values vector contains values that are not between 0 and 1
			*/
			void addPrecomputeDoseValues(const std::vector<double>& values);
			DoseStatistics::DoseToVolumeMeasureCollectionPointer getMeasureCollection();

		protected:
			DoseToVolumeMeasureCollectionCalculator(const std::vector<double>& precomputeDoseValues,
				const DoseTypeGy referenceDose, const core::DoseIteratorInterface::DoseIteratorPointer doseIterator, 
				DoseToVolumeMeasureCollection::complexStatistics name, bool multiThreading);

			void insertIntoMeasureCollection(DoseTypeGy xAbsolute, VolumeType resultVolume);

			/*! @brief Computes the specific VolumeType depending on the complext statistic
			@note has to be overwritten
			*/
			virtual VolumeType computeSpecificValue(double xAbsolute) const = 0;

		private:
			bool isInVector(double value);
		};

	}
}


#endif
