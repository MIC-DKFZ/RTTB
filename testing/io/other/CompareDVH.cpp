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

#include <boost/make_shared.hpp>

namespace rttb
{

	namespace testing
	{

		bool checkEqualDVH(DVHPointer aDVH1, DVHPointer aDVH2)
		{

			bool result;
			const double errorConstantDVH = 1e-4;
			result = lit::AreClose(aDVH1->getDeltaD(), aDVH2->getDeltaD(), errorConstantDVH);
			result = result && lit::AreClose(aDVH1->getDeltaV(), aDVH2->getDeltaV(), errorConstantDVH);
			result = result && (aDVH1->getDoseID() == aDVH2->getDoseID());
			result = result && (aDVH1->getStructureID() == aDVH2->getStructureID());
			result = result && lit::AreClose(aDVH1->getMaximum(), aDVH2->getMaximum(), errorConstantDVH);
			result = result && lit::AreClose(aDVH1->getMinimum(), aDVH2->getMinimum(), errorConstantDVH);
			result = result && lit::AreClose(aDVH1->getMean(), aDVH2->getMean(), errorConstantDVH);
			result = result && (aDVH1->getDataDifferential().size() == aDVH2->getDataDifferential().size());

			for (size_t i = 0; i < aDVH1->getDataDifferential().size(); i++)
			{
				result = result
				         && lit::AreClose(aDVH1->getDataDifferential().at(i), aDVH2->getDataDifferential().at(i),
							 errorConstantDVH);
			}

			return result;

		}

        rttb::testing::DVHPointer computeDiffDVH(DVHPointer aDVH1, DVHPointer aDVH2)
        {
            if (aDVH1->getDeltaD() == aDVH2->getDeltaD() && aDVH1->getDeltaV() == aDVH2->getDeltaV()){

                rttb::core::DVH::DataDifferentialType dvhData1 = aDVH1->getDataDifferential();
                rttb::core::DVH::DataDifferentialType dvhData2 = aDVH2->getDataDifferential();
                rttb::core::DVH::DataDifferentialType dvhDataDifference;

                auto it1 = dvhData1.cbegin();
                auto it2 = dvhData2.cbegin();

                while (it1 != dvhData1.cend() && it2 != dvhData2.cend())
                {
                    dvhDataDifference.push_back(*it1-*it2);
                    ++it1;
                    ++it2;
                }

                auto differenceDVH = ::boost::make_shared<core::DVH>(dvhDataDifference, aDVH1->getDeltaD(), aDVH1->getDeltaV(), aDVH1->getStructureID(), aDVH1->getDoseID());
                return differenceDVH;
            }
            else {
                return aDVH1;
            }
        }

    }//testing
}//rttb

