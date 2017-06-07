#include "rttbMeasureInterface.h"

namespace rttb
{

	namespace algorithms
	{
		MeasureInterface::complexStatistics MeasureInterface::getName() const
		{
			return this->name;
		}

		std::map<double, double>::const_iterator MeasureInterface::findNearestKeyInMap(
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