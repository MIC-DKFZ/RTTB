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

#include "rttbVxDoseToVolumeMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		VxDoseToVolumeMeasureCollectionCalculator::VxDoseToVolumeMeasureCollectionCalculator(const std::vector<double>& precomputeDoseValues,
			const DoseTypeGy referenceDose, const core::DoseIteratorInterface::DoseIteratorPointer doseIterator, bool multiThreading) :
			DoseToVolumeMeasureCollectionCalculator(precomputeDoseValues, referenceDose, doseIterator, DoseToVolumeMeasureCollection::Vx, multiThreading) {}

		VolumeType VxDoseToVolumeMeasureCollectionCalculator::computeSpecificValue(double xAbsolute) const
		{

			rttb::FractionType count = 0;
			_doseIterator->reset();

			DoseTypeGy currentDose = 0;

			while (_doseIterator->isPositionValid())
			{
				currentDose = _doseIterator->getCurrentDoseValue();

				if (currentDose >= xAbsolute)
				{
					count += _doseIterator->getCurrentRelevantVolumeFraction();
				}

				_doseIterator->next();
			}
			return count * this->_doseIterator->getCurrentVoxelVolume();
		}
	}
}