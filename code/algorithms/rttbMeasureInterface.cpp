#include "rttbMeasureInterface.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{

	namespace algorithms
	{
		MeasureInterface::complexStatistics MeasureInterface::getName() const
		{
			return this->name;
		}

		double MeasureInterface::getSpecificValue(const std::map<double, double>& values, double key, bool findNearestValueInstead, double& storedKey) const
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

		std::map<double, double>::const_iterator MeasureInterface::findNearestKeyInMap(
			const std::map<double, double>& values,
			double key) const
		{
			double minDistance = 1e19;
			double minDistanceLast = 1e20;

			auto iterator = std::begin(values);

			while (iterator != std::end(values))
			{
				minDistanceLast = minDistance;
				minDistance = fabs(iterator->first - key);

				if (minDistanceLast > minDistance)
				{
					++iterator;
				}
				else
				{
					if (iterator != std::begin(values))
					{
						--iterator;
						return iterator;
					}
					else
					{
						return std::begin(values);
					}
				}
			}

			--iterator;
			return iterator;
		}
	}
}