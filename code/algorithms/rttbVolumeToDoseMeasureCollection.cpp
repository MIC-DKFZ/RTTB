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


#include "rttbVolumeToDoseMeasureCollection.h"
#include "rttbInvalidParameterException.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasureCollection::VolumeToDoseMeasureCollection(complexStatistics name, VolumeType volume) :
			_name(name), _values(std::map<VolumeType, DoseTypeGy>()), _volume(volume) {}

		void VolumeToDoseMeasureCollection::setVolume(VolumeType volume)
		{
			this->_volume = volume;
		}

		void VolumeToDoseMeasureCollection::insertValue(VolumeType volume, DoseTypeGy dose)
		{
			this->_values.insert(std::pair<VolumeType, DoseTypeGy>(volume, dose));
		}

		DoseTypeGy VolumeToDoseMeasureCollection::getValue(VolumeType xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getSpecificValue(_values, xVolumeAbsolute, false, dummy);
		}

		DoseTypeGy VolumeToDoseMeasureCollection::getValue(VolumeType xVolumeAbsolute, bool findNearestValue, VolumeType & nearestXVolume) const
		{
			return getSpecificValue(_values, xVolumeAbsolute, findNearestValue, nearestXVolume);
		}

		DoseTypeGy VolumeToDoseMeasureCollection::getValueRelative(VolumeType xVolumeRelative) const
		{
			if (_volume == -1) {
				throw rttb::core::DataNotAvailableException("Volume is not set");
			}
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative * _volume;
				VolumeType dummy;
				return getSpecificValue(_values, xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy VolumeToDoseMeasureCollection::getValueRelative(VolumeType xVolumeRelative, bool findNearestValue, VolumeType & nearestXVolume) const
		{
			if (_volume == -1) {
				throw rttb::core::DataNotAvailableException("Volume is not set");
			}
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative * _volume;
				return getSpecificValue(_values, xVolumeAbsolute, findNearestValue, nearestXVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		VolumeToDoseMeasureCollection::VolumeToDoseFunctionType VolumeToDoseMeasureCollection::getAllValues() const
		{
			return this->_values;
		}

		bool VolumeToDoseMeasureCollection::isInCollection(VolumeType value) const
		{
			double doubleImprecision = 1e-5;
			for (auto const& collectionElements : _values) {
				if (collectionElements.first - doubleImprecision <= value && value <= collectionElements.first + doubleImprecision)
				{
					return true;
				}
			}
			return false;
		}

		bool operator==(const VolumeToDoseMeasureCollection& volumeToDoseMesureCollection,const VolumeToDoseMeasureCollection& otherVolumeToDoseMesureCollection)
		{
			if (volumeToDoseMesureCollection.getName() == otherVolumeToDoseMesureCollection.getName() && volumeToDoseMesureCollection.getAllValues() == otherVolumeToDoseMesureCollection.getAllValues()) {
				return true;
			}
			return false;
		}
	}
}
