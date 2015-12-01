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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/


#include "CompareDVH.h"



namespace rttb
{

	namespace testing
	{

		bool checkEqualDVH(DVHPointer aDVH1, DVHPointer aDVH2)
		{

			bool result;
			const double errorConstant = 1e-7;
			result = lit::AreClose(aDVH1->getDeltaD(), aDVH2->getDeltaD(), errorConstant);
			result = result && lit::AreClose(aDVH1->getDeltaV(), aDVH2->getDeltaV(), errorConstant);
			result = result && (aDVH1->getDoseID() == aDVH2->getDoseID());
			result = result && (aDVH1->getStructureID() == aDVH2->getStructureID());
			result = result && lit::AreClose(aDVH1->getMaximum(), aDVH2->getMaximum(), errorConstant);
			result = result && lit::AreClose(aDVH1->getMinimum(), aDVH2->getMinimum(), errorConstant);
			result = result && lit::AreClose(aDVH1->getMean(), aDVH2->getMean(), errorConstant);
			result = result && (aDVH1->getDataDifferential().size() == aDVH2->getDataDifferential().size());

			for (int i = 0; i < aDVH1->getDataDifferential().size(); i++)
			{
				result = result
				         && lit::AreClose(aDVH1->getDataDifferential().at(i), aDVH2->getDataDifferential().at(i),
				                          errorConstant);
			}

			return result;

		}

	}//testing
}//rttb

