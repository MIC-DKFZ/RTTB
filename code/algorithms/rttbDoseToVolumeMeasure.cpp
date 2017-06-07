#include "rttbDoseToVolumeMeasure.h"
#include "rttbDataNotAvailableException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace algorithms
	{
		DoseToVolumeMeasure::DoseToVolumeMeasure(complexStatistics name, DoseToVolumeFunctionType values, DoseTypeGy referenceDose) :
			name(name), values(values), _referenceDose(referenceDose) {}
		
		void DoseToVolumeMeasure::insertValue(std::pair<VolumeType, DoseTypeGy> value)
		{
			this->values.insert(value);
		}

		VolumeType DoseToVolumeMeasure::getValue(DoseTypeGy xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getSpecificValue(xVolumeAbsolute, false, dummy);
		}

		VolumeType DoseToVolumeMeasure::getValue(DoseTypeGy xDoseAbsolute, bool findNearestValue, DoseTypeGy & nearestXDose) const
		{
			return getSpecificValue(xDoseAbsolute, findNearestValue, nearestXDose);
		}
		VolumeType DoseToVolumeMeasure::getValueRelative(DoseTypeGy xDoseRelative) const
		{
			if (_referenceDose != -1 && xDoseRelative >= 0 && xDoseRelative <= 1) {
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				DoseTypeGy dummy;
				return getSpecificValue(xDoseAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}
		VolumeType DoseToVolumeMeasure::getValueRelative(DoseTypeGy xDoseRelative, bool findNearestValue, DoseTypeGy & nearestXDose) const
		{
			if (_referenceDose != -1 && xDoseRelative >= 0 && xDoseRelative <= 1) {
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				return getSpecificValue(xDoseAbsolute, findNearestValue, nearestXDose);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}
		DoseToVolumeMeasure::DoseToVolumeFunctionType DoseToVolumeMeasure::getAllValues() const
		{
			return this->values;
		}

		void DoseToVolumeMeasure::setReferenceDose(DoseTypeGy referenceDose)
		{
			this->_referenceDose = referenceDose;
		}

		double DoseToVolumeMeasure::getSpecificValue(double key, bool findNearestValueInstead, double& storedKey) const
		{
			if (values.find(key) != std::end(values))
			{
				return values.find(key)->second;
			}
			else
			{
				//value not in map. We have to find the nearest value
				if (values.empty())
				{
					throw core::DataNotAvailableException("No Vx values are defined");
				}
				else
				{
					if (findNearestValueInstead)
					{
						auto iterator = findNearestKeyInMap(values, key);
						storedKey = iterator->first;
						return iterator->second;
					}
					else
					{
						throw core::DataNotAvailableException("No Vx value with required dose is defined");
					}
				}
			}
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
