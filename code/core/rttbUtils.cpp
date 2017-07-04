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

#ifndef __RTTB_UTILS_H
#define __RTTB_UTILS_H

#include <map>
#include <rttbBaseType.h>

namespace rttb
{
	namespace core
	{

		static bool isKey(std::map<double, double> values, double value) {
			for (auto const& collectionElements : values) {
				if (std::abs(collectionElements.first - value) <= rttb::errorConstant)
				{
					return true;
				}
			}
			return false;
		}

		static bool isKey(std::vector<double> values, double value) {
			for (auto const& collectionElement : values) {
				if (std::abs(collectionElement - value) <= rttb::errorConstant)
				{
					return true;
				}
			}
			return false;
		}
	}
}
#endif
