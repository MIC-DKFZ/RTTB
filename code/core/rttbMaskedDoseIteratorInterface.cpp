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

#include "rttbMaskedDoseIteratorInterface.h"
#include "rttbNullPointerException.h"

namespace rttb
{
	namespace core
	{
		MaskedDoseIteratorInterface::MaskedDoseIteratorInterface(MaskAccessorPointer aMaskAccessor,
		        DoseAccessorPointer aDoseAccessor): DoseIteratorInterface(aDoseAccessor)
		{
			if (aMaskAccessor == nullptr)
			{
				throw NullPointerException(" mask pointer must not be nullptr!");
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