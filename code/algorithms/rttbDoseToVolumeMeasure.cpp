#include "rttbDoseToVolumeMeasure.h"
#include "rttbInvalidParameterException.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{

	namespace algorithms
	{
		DoseToVolumeMeasure::DoseToVolumeMeasure(complexStatistics name, DoseTypeGy referenceDose) :
			name(name), values(std::map<VolumeType, DoseTypeGy>()), _referenceDose(referenceDose) {}

		void DoseToVolumeMeasure::setReferenceDose(DoseTypeGy referenceDose)
		{
			this->_referenceDose = referenceDose;
		}

		void DoseToVolumeMeasure::insertValue(DoseTypeGy dose, VolumeType volume)
		{
			this->values.insert(std::pair<DoseTypeGy, VolumeType>(dose, volume));
		}

		VolumeType DoseToVolumeMeasure::getValue(DoseTypeGy xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getSpecificValue(values, xVolumeAbsolute, false, dummy);
		}

		VolumeType DoseToVolumeMeasure::getValue(DoseTypeGy xDoseAbsolute, bool findNearestValue, DoseTypeGy & nearestXDose) const
		{
			return getSpecificValue(values, xDoseAbsolute, findNearestValue, nearestXDose);
		}

		VolumeType DoseToVolumeMeasure::getValueRelative(DoseTypeGy xDoseRelative) const
		{
			if (_referenceDose != -1 && xDoseRelative >= 0 && xDoseRelative <= 1) {
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				DoseTypeGy dummy;
				return getSpecificValue(values, xDoseAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}

		VolumeType DoseToVolumeMeasure::getValueRelative(DoseTypeGy xDoseRelative, bool findNearestValue, DoseTypeGy & nearestXDose) const
		{
			if (_referenceDose != -1 && xDoseRelative >= 0 && xDoseRelative <= 1) {
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				return getSpecificValue(values, xDoseAbsolute, findNearestValue, nearestXDose);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}

		DoseToVolumeMeasure::DoseToVolumeFunctionType DoseToVolumeMeasure::getAllValues() const
		{
			return this->values;
		}

		bool operator==(const DoseToVolumeMeasure& volumeToDoseMesure,const DoseToVolumeMeasure& otherVolumeToDoseMesure)
		{
			if (volumeToDoseMesure.getName() == otherVolumeToDoseMesure.getName() && volumeToDoseMesure.getAllValues() == otherVolumeToDoseMesure.getAllValues()) {
				return true;
			}
			return false;
		}
	}
}
