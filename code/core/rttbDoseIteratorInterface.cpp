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

#include "rttbDoseIteratorInterface.h"
#include "rttbNullPointerException.h"

namespace rttb
{
	namespace core
	{

		DoseIteratorInterface::DoseIteratorInterface(DoseAccessorPointer aDoseAccessor)
		{
			if (aDoseAccessor == nullptr)
			{
				throw NullPointerException(" dose accessor pointer must not be nullptr!");
			}

			_spDoseAccessor = aDoseAccessor;
		}
	}//end: namespace core
}//end: namespace rttb

