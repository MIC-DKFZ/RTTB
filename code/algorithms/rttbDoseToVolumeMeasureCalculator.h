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

#ifndef __DOSE_TO_VOLUME_MEASURE_CALCULATOR_H
#define __DOSE_TO_VOLUME_MEASURE_CALCULATOR_H

#include <vector>
#include <map>

#include "rttbBaseType.h"


#include "RTTBAlgorithmsExports.h"
#include "rttbDoseToVolumeMeasure.h"

#include "rttbDoseIteratorInterface.h"

namespace rttb
{

	namespace algorithms
	{
		class RTTBAlgorithms_EXPORT DoseToVolumeMeasureCalculator {

		public:
			typedef std::map<DoseTypeGy, VolumeType> VolumeToDoseFunctionType;

		protected:
			core::DoseIteratorInterface::DoseIteratorPointer _doseIterator;

		private:
			DoseTypeGy _referenceDose;
			DoseToVolumeMeasure measure;
			std::vector<double> _precomputeVolumeValues;

		public:
			DoseToVolumeMeasureCalculator(const std::vector<double>& precomputeVolumeValues,
				const DoseTypeGy& referenceDose, const core::DoseIteratorInterface::DoseIteratorPointer doseIterator, DoseToVolumeMeasure::complexStatistics name);
			void compute();
			DoseToVolumeMeasure getMeasure();
			virtual void computeSpecificValue(double xAbsolute) = 0;

		protected:
			void insertIntoMeasure(DoseTypeGy xAbsolute, VolumeType resultVolume);
		};

	}
}


#endif
