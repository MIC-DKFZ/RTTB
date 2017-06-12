#include "rttbVolumeToDoseMeasure.h"
#include "rttbInvalidParameterException.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasure::VolumeToDoseMeasure(complexStatistics name, VolumeType volume) :
			name(name), values(std::map<VolumeType, DoseTypeGy>()), _volume(volume) {}

		void VolumeToDoseMeasure::setVolume(VolumeType volume)
		{
			this->_volume = volume;
		}

		void VolumeToDoseMeasure::insertValue(VolumeType volume, DoseTypeGy dose)
		{
			this->values.insert(std::pair<VolumeType, DoseTypeGy>(volume, dose));
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
			if (_volume == -1) {
				throw rttb::core::DataNotAvailableException("Volume is not set");
			}
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
			if (_volume == -1) {
				throw rttb::core::DataNotAvailableException("Volume is not set");
			}
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
