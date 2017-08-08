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

#ifndef __VOLUME_TO_DOSE_MEASURE_COLLECTION_CALCULATOR_H
#define __VOLUME_TO_DOSE_MEASURE_COLLECTION_CALCULATOR_H

#include "rttbVolumeToDoseMeasureCollection.h"
#include <boost/shared_ptr.hpp>
#include "rttbDoseStatistics.h"

namespace rttb
{

	namespace algorithms
	{
		/*! @class VolumeToDoseMeasureCollectionCalculator
		@brief Class for calculating VolumeToDose measures
		@details This class is used as a basis for VolumeToDoseMeasureCollectionCalculator. It implements the compute() method
		that is the same for every inheriting subclass, it accesses computeSpecificValue, which has to be implemented
		for each different complex statistic.
		*/
		class RTTBAlgorithms_EXPORT VolumeToDoseMeasureCollectionCalculator {

		public:
			typedef std::map<VolumeType, DoseTypeGy> VolumeToDoseFunctionType;

		protected:
			std::vector<DoseTypeGy> _doseVector;
			DoseVoxelVolumeType _currentVoxelVolume;
			std::vector<double> _voxelProportionVector;

		private:
      std::vector<double> _precomputeVolumeValues;
			VolumeType _volume;
			DoseStatistics::VolumeToDoseMeasureCollectionPointer _measureCollection;
			bool _multiThreading;

		public:
			/*!  @brief Computes not already computed values for the measureCollection. Algorithm for the specific complex Statistic has to be implemented in the corresponding subclass.
			*/
			void compute();
			/*! @brief Adds additional values to the _precomputeVolumeValues vector.
			@exception InvalidParameterException If values vector contains values that are not between 0 and 1
			*/
			void addPrecomputeVolumeValues(const std::vector<double>& values);
			DoseStatistics::VolumeToDoseMeasureCollectionPointer getMeasureCollection();

		protected:
			VolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues, const VolumeType volume,
				const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
				const DoseVoxelVolumeType currentVoxelVolume, VolumeToDoseMeasureCollection::complexStatistics name, bool multiThreading);

			void insertIntoMeasureCollection(VolumeType xAbsolute, DoseTypeGy resultDose);

			/*! @brief Computes the specific DoseTypeGy depending on the complext statistic
			@note has to be overwritten
			*/
			virtual DoseTypeGy computeSpecificValue(double xAbsolute) const = 0;
		};

	}
}


#endif
