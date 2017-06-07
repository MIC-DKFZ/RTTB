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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include "CompareDoseStatistic.h"



namespace rttb
{

	namespace testing
	{
		const double errorConstant = 1e-3;// errorConstant is so small because the double are castet to float when they are written
		bool checkEqualDoseStatistic(DoseStatisticsPtr aDoseStatistc1, DoseStatisticsPtr aDoseStatistic2){
			bool result = lit::AreClose(aDoseStatistc1->getNumberOfVoxels(), aDoseStatistic2->getNumberOfVoxels(), 0.01);
			result = result &&  lit::AreClose(aDoseStatistc1->getVolume(), aDoseStatistic2->getVolume(), errorConstant);
			result = result &&  lit::AreClose(aDoseStatistc1->getMinimum(), aDoseStatistic2->getMinimum(), errorConstant);
			result = result &&  lit::AreClose(aDoseStatistc1->getMaximum(), aDoseStatistic2->getMaximum(), errorConstant);
			result = result &&  lit::AreClose(aDoseStatistc1->getMean(), aDoseStatistic2->getMean(), errorConstant);
			result = result &&  lit::AreClose(aDoseStatistc1->getStdDeviation(), aDoseStatistic2->getStdDeviation(), errorConstant);
			result = result && mapCompare(aDoseStatistc1->getDx().getAllValues(), aDoseStatistic2->getDx().getAllValues());
			result = result && mapCompare(aDoseStatistc1->getVx().getAllValues(), aDoseStatistic2->getVx().getAllValues());
			result = result && mapCompare(aDoseStatistc1->getMaxOHx().getAllValues(), aDoseStatistic2->getMaxOHx().getAllValues());
			result = result && mapCompare(aDoseStatistc1->getMinOCx().getAllValues(), aDoseStatistic2->getMinOCx().getAllValues());
			result = result && mapCompare(aDoseStatistc1->getMOCx().getAllValues(), aDoseStatistic2->getMOCx().getAllValues());
			result = result && mapCompare(aDoseStatistc1->getMOHx().getAllValues(), aDoseStatistic2->getMOHx().getAllValues());

			return result;
		}

        std::map<double, double>::const_iterator findNearestKeyInMap(const std::map<double, double>& aMap, double key) {
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


        double getValue(const std::map<double, double>& aMap, double key) {
            if (aMap.find(key) != std::end(aMap))
            {
                return aMap.find(key)->second;
            }
            else
            {
                auto iterator = findNearestKeyInMap(aMap, key);
                return iterator->second;
            }
        }


		bool mapCompare(const std::map<double, double>& lhs, const std::map<double, double>& rhs) {
			if (lhs.size() != rhs.size()){
				return false;
			}

			for (std::map<double, double>::const_iterator i = rhs.cbegin(); i != rhs.cend(); ++i)
			{
				double a = i->second;
				double b = getValue(lhs, i->first) ;
				if (std::abs(a - b ) > errorConstant){// errorConstant is 1e-3 because the double-->float cast when they are written
					return false;
				}
			}
			return true;

		}

	}//testing
}//rttb

