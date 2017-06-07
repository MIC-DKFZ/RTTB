#include "rttbVolumeToDoseMeasure.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasure::VolumeToDoseMeasure(complexStatistics name, VolumeToDoseFunctionType values, VolumeType volume) :
			name(name), values(values), _volume(volume) {}

		void VolumeToDoseMeasure::setVolume(VolumeType volume)
		{
			this->_volume = volume;
		}

		void VolumeToDoseMeasure::insertValue(std::pair<VolumeType, DoseTypeGy> value)
		{
			this->values.insert(value);
		}

		DoseTypeGy VolumeToDoseMeasure::getValue(VolumeType xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getSpecificValue(values, xVolumeAbsolute, false, dummy);
		}

		DoseTypeGy VolumeToDoseMeasure::getValue(VolumeType xVolumeAbsolute, bool findNearestValue, VolumeType & nearestXVolume) const
		{
			return getSpecificValue(values, xVolumeAbsolute, findNearestValue, nearestXVolume);
		}

		DoseTypeGy VolumeToDoseMeasure::getValueRelative(VolumeType xVolumeRelative) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative * _volume;
				VolumeType dummy;
				return getSpecificValue(values, xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy VolumeToDoseMeasure::getValueRelative(VolumeType xVolumeRelative, bool findNearestValue, VolumeType & nearestXVolume) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative * _volume;
				return getSpecificValue(values, xVolumeAbsolute, findNearestValue, nearestXVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		VolumeToDoseMeasure::VolumeToDoseFunctionType VolumeToDoseMeasure::getAllValues() const
		{
			return this->values;
		}

		bool operator==(const VolumeToDoseMeasure& volumeToDoseMesure,const VolumeToDoseMeasure& otherVolumeToDoseMesure)
		{
			if (volumeToDoseMesure.getName() == otherVolumeToDoseMesure.getName() && volumeToDoseMesure.getAllValues() == otherVolumeToDoseMesure.getAllValues()) {
				return true;
			}
			return false;
		}
	}
}
