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


#include "litCheckMacros.h"

#include "rttbDoseStatistics.h"

#ifndef __DOSE_STATISTICS_COMPARER_H
#define __DOSE_STATISTICS_COMPARER_H


namespace rttb
{

	namespace testing
	{
		typedef boost::shared_ptr<rttb::algorithms::DoseStatistics> DoseStatisticsPtr;

		/*! Compare 2 DoseStatistics and return the results.
		@result Indicates if the test was successful (true) or if it failed (false)
		*/
		bool checkEqualDoseStatistic(DoseStatisticsPtr aDoseStatistic1, DoseStatisticsPtr aDoseStatistic2);

		std::map<double, double>::const_iterator findNearestKeyInMap(const std::map<double, double>& aMap, double key);

		double getValue(const std::map<double, double>& aMap, double key);

		bool mapCompare(const std::map<double, double>& lhs, const std::map<double, double>& rhs);

	}//testing
}//rttb
#endif
