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

#include "rttbDvhBasedDoseIndex.h"

namespace rttb
{
	namespace indices
	{

		DvhBasedDoseIndex::DvhBasedDoseIndex(core::DVHSet::Pointer aDVHSet,
		                                     DoseTypeGy aDoseReference)
			: DoseIndex(aDoseReference), _dvhSet(aDVHSet)
		{
		}

		bool DvhBasedDoseIndex::checkInputs()
		{
			if (!_dvhSet)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
}


