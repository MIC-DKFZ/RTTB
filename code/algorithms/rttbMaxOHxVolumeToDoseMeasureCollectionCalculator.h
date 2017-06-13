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

#ifndef __MAXOHX_VOLUME_TO_DOSE_MEASURE_CALCULATOR_COLLECTION_H
#define __MAXOHX_VOLUME_TO_DOSE_MEASURE_CALCULATOR_COLLECTION_H

#include "rttbVolumeToDoseMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		/*! @class MaxOHxVolumeToDoseMeasureCollectionCalculator
		@brief Class for calculating MaxOH VolumeToDose measures
		*/
		class RTTBAlgorithms_EXPORT MaxOHxVolumeToDoseMeasureCollectionCalculator : public VolumeToDoseMeasureCollectionCalculator {

		private:

		public:
			MaxOHxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues, const VolumeType volume,
				const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
				const DoseVoxelVolumeType currentVoxelVolume, VolumeToDoseMeasureCollection::complexStatistics name);

		protected:
			DoseTypeGy computeSpecificValue(double xAbsolute) const ;
		};
	}
}


#endif
