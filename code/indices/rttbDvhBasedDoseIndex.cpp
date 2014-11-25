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
// @version $Revision: 707 $ (last changed revision)
// @date    $Date: 2014-09-04 16:37:24 +0200 (Do, 04 Sep 2014) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#include "rttbDvhBasedDoseIndex.h"


namespace rttb{
	namespace indices{

		DvhBasedDoseIndex::DvhBasedDoseIndex(DvhBasedDoseIndex::DVHSetPtr aDVHSet, DoseTypeGy aDoseReference)
		:DoseIndex(aDoseReference),_dvhSet(aDVHSet)
		{	
		}

		bool DvhBasedDoseIndex::checkInputs(){
			if(!_dvhSet){
				return false;
			}
			else{
				return true;
			}
		}
	}
}


