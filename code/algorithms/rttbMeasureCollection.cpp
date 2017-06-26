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

#include "rttbMeasureCollection.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{

	namespace algorithms
	{
		MeasureCollection::complexStatistics MeasureCollection::getName() const
		{
			return this->_name;
		}

		double MeasureCollection::getSpecificValue(const std::map<double, double>& values, double key, bool findNearestValueInstead, double& storedKey) const
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

		std::map<double, double>::const_iterator MeasureCollection::findNearestKeyInMap(
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