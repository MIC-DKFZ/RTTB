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

#include "rttbMaskedDoseIteratorInterface.h"
#include "rttbNullPointerException.h"
#include "rttbException.h"

namespace rttb
{
	namespace core
	{
		MaskedDoseIteratorInterface::MaskedDoseIteratorInterface(MaskAccessorPointer aMaskAccessor,
		        DoseAccessorPointer aDoseAccessor): DoseIteratorInterface(aDoseAccessor)
		{
			if (! aMaskAccessor)
			{
				throw NullPointerException(" mask pointer must not be NULL!");
			}
			else
			{
				_spMask = aMaskAccessor;
			}

			if (!(_spMask->getGeometricInfo() == _spDoseAccessor->getGeometricInfo()))
			{
				throw Exception("Mask and Dose need to be defined on the same grid");
			}
		}
	}
}