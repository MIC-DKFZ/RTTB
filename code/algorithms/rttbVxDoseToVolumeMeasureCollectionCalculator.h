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

#ifndef __DV_DOSE_TO_VOLUME_MEASURE_CALCULATOR_H
#define __DV_DOSE_TO_VOLUME_MEASURE_CALCULATOR_H

#include "rttbDoseToVolumeMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		/*! @class VxDoseToVolumeMeasureCollectionCalculator
		@brief Class for calculating Vx DoseToVolume measures
		*/
		class RTTBAlgorithms_EXPORT VxDoseToVolumeMeasureCollectionCalculator : public DoseToVolumeMeasureCollectionCalculator {

		public:			
			VxDoseToVolumeMeasureCollectionCalculator(const std::vector<double>& precomputeDoseValues,
				const DoseTypeGy referenceDose, const core::DoseIteratorInterface::Pointer doseIterator,
				bool multiThreading = false);

		protected:
			VolumeType computeSpecificValue(double xAbsolute) const override;
		};
	}
}


#endif
