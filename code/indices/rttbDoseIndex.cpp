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

#include "rttbDoseIndex.h"
#include "rttbException.h"

namespace rttb{
	namespace indices{
		void DoseIndex::setDoseReference(DoseTypeGy aDoseReference)
			{
			_doseReference=aDoseReference;
			initSuccess=false;
			}

		DoseTypeGy DoseIndex::getDoseReference() const
			{
			return _doseReference;
			}

		IndexValueType DoseIndex::getValue() const
			{
			if(initSuccess){
				return _value;
				}
			else{
        throw core::Exception("DoseIndex init error: init() must be called first!");
				}
			}
		}
	}


