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

#include "rttbDoseToVolumeMeasureCollection.h"
#include "rttbInvalidParameterException.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{

	namespace algorithms
	{
		DoseToVolumeMeasureCollection::DoseToVolumeMeasureCollection(complexStatistics name, DoseTypeGy referenceDose) :
			_name(name), _values(std::map<VolumeType, DoseTypeGy>()), _referenceDose(referenceDose) {}

		void DoseToVolumeMeasureCollection::setReferenceDose(DoseTypeGy referenceDose)
		{
			this->_referenceDose = referenceDose;
		}

		void DoseToVolumeMeasureCollection::insertValue(DoseTypeGy dose, VolumeType volume)
		{
			this->_values.insert(std::pair<DoseTypeGy, VolumeType>(dose, volume));
		}

		VolumeType DoseToVolumeMeasureCollection::getValue(DoseTypeGy xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getSpecificValue(_values, xVolumeAbsolute, false, dummy);
		}

		VolumeType DoseToVolumeMeasureCollection::getValue(DoseTypeGy xDoseAbsolute, bool findNearestValue, DoseTypeGy & nearestXDose) const
		{
			return getSpecificValue(_values, xDoseAbsolute, findNearestValue, nearestXDose);
		}

		VolumeType DoseToVolumeMeasureCollection::getValueRelative(DoseTypeGy xDoseRelative) const
		{
			if (_referenceDose != -1 && xDoseRelative >= 0 && xDoseRelative <= 1) {
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				DoseTypeGy dummy;
				return getSpecificValue(_values, xDoseAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}

		VolumeType DoseToVolumeMeasureCollection::getValueRelative(DoseTypeGy xDoseRelative, bool findNearestValue, DoseTypeGy & nearestXDose) const
		{
			if (_referenceDose != -1 && xDoseRelative >= 0 && xDoseRelative <= 1) {
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				return getSpecificValue(_values, xDoseAbsolute, findNearestValue, nearestXDose);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}

		DoseToVolumeMeasureCollection::DoseToVolumeFunctionType DoseToVolumeMeasureCollection::getAllValues() const
		{
			return this->_values;
		}

		bool operator==(const DoseToVolumeMeasureCollection& volumeToDoseMesureCollection, const DoseToVolumeMeasureCollection& otherVolumeToDoseMesureCollection)
		{
			if (volumeToDoseMesureCollection.getName() == otherVolumeToDoseMesureCollection.getName() && volumeToDoseMesureCollection.getAllValues() == otherVolumeToDoseMesureCollection.getAllValues()) {
				return true;
			}
			return false;
		}
	}
}
