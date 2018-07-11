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

#include <rttbCommon.h>
#include "rttbDoseStatistics.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

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
      rttbClassMacroNoParent(DoseToVolumeMeasureCollectionCalculator)
			typedef std::map<DoseTypeGy, VolumeType> VolumeToDoseFunctionType;

		protected:
			core::DoseIteratorInterface::Pointer _doseIterator;

		private:
      std::vector<double> _precomputeDoseValues;
			DoseTypeGy _referenceDose;
			DoseToVolumeMeasureCollection::Pointer _measureCollection;
			bool _multiThreading;

		public:
			/*! @brief Computes not already computed values for the measureCollection. Algorithm for the specific complex Statistic has to be implemented in the corresponding subclass.
			*/
			void compute();
			/*! @brief Adds additional values to the _precomputeDoseValues vector.
				@exception InvalidParameterException If values vector contains values that are not between 0 and 1
			*/
			void addPrecomputeDoseValues(const std::vector<double>& values);
			DoseToVolumeMeasureCollection::Pointer getMeasureCollection();

		protected:
			DoseToVolumeMeasureCollectionCalculator(const std::vector<double>& precomputeDoseValues,
				const DoseTypeGy referenceDose, const core::DoseIteratorInterface::Pointer doseIterator, 
				DoseToVolumeMeasureCollection::complexStatistics name, bool multiThreading);

			void insertIntoMeasureCollection(DoseTypeGy xAbsolute, VolumeType resultVolume);

			/*! @brief Computes the specific VolumeType depending on the complext statistic
			@note has to be overwritten
			*/
			virtual VolumeType computeSpecificValue(double xAbsolute) const = 0;
		};

	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
