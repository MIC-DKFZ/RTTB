#include "rttbVolumeToDoseMeasure.h"
#include "rttbDataNotAvailableException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasure::VolumeToDoseMeasure(DoseStatistics::complexStatistics name, VolumeToDoseFunctionType values) :
			name(name), values(values) {}

		DoseStatistics::complexStatistics VolumeToDoseMeasure::getName()
		{
			return this->name;
		}

		void VolumeToDoseMeasure::insertValue(std::pair<VolumeType, DoseTypeGy> value)
		{
			this->values.insert(value);
		}

		DoseTypeGy VolumeToDoseMeasure::getValue(VolumeType xVolumeAbsolute)
		{
			VolumeType dummy;
			return getValue(xVolumeAbsolute, false, dummy);
			return DoseTypeGy();
		}
		DoseTypeGy VolumeToDoseMeasure::getValue(VolumeType xVolumeAbsolute, bool findNearestValue, VolumeType & nearestXVolume)
		{
			return getValue(xVolumeAbsolute, findNearestValue, nearestXVolume);
			return DoseTypeGy();
		}
		DoseTypeGy VolumeToDoseMeasure::getValueRelative(VolumeType xVolumeRelative)
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
				VolumeType dummy;
				return getValue(xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
			return DoseTypeGy();
		}
		DoseTypeGy VolumeToDoseMeasure::getValueRelative(VolumeType xVolumeRelative, bool findNearestValue, VolumeType & nearestXVolume)
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
				return getValue(xVolumeAbsolute, findNearestValue, nearestXVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
			return DoseTypeGy();
		}
		VolumeToDoseMeasure::VolumeToDoseFunctionType VolumeToDoseMeasure::getAllValues()
		{
			return this->values;
		}

		double VolumeToDoseMeasure::getValue(double key,
			bool findNearestValueInstead, double& storedKey) const
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
		std::map<double, double>::const_iterator VolumeToDoseMeasure::findNearestKeyInMap(
			const std::map<double, double>& aMap,
			double key) const
		{
			double minDistance = 1e19;
			double minDistanceLast = 1e20;

			auto iterator = std::begin(aMap);

			while (iterator != std::end(aMap))
			{
				minDistanceLast = minDistance;
				minDistance = fabs(iterator->first - key);

				if (minDistanceLast > minDistance)
				{
					++iterator;
				}
				else
				{
					if (iterator != std::begin(aMap))
					{
						--iterator;
						return iterator;
					}
					else
					{
						return std::begin(aMap);
					}
				}
			}

			--iterator;
			return iterator;
		}
	}
}
