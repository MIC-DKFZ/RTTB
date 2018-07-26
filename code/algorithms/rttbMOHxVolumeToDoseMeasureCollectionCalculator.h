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

#ifndef __MOHX_VOLUME_TO_DOSE_MEASURE_COLLECTION_CALCULATOR_H
#define __MOHX_VOLUME_TO_DOSE_MEASURE_COLLECTION_CALCULATOR_H

#include "rttbVolumeToDoseMeasureCollectionCalculator.h"

#include <rttbCommon.h>

namespace rttb
{

	namespace algorithms
	{
		/*! @class MOHxVolumeToDoseMeasureCollectionCalculator
		@brief Class for calculating MOHx VolumeToDose measures
		*/
		class RTTBAlgorithms_EXPORT MOHxVolumeToDoseMeasureCollectionCalculator : public VolumeToDoseMeasureCollectionCalculator {
    public:
      rttbClassMacro(MOHxVolumeToDoseMeasureCollectionCalculator, VolumeToDoseMeasureCollectionCalculator)
		private:
			DoseStatisticType _minimum;

		public:
			MOHxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues, const VolumeType volume,
				const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
				const DoseVoxelVolumeType currentVoxelVolume, bool multiThreading = false);

		protected:
			DoseTypeGy computeSpecificValue(double xAbsolute) const override;
		};
	}
}


#endif
