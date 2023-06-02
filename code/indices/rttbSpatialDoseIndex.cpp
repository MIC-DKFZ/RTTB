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

#include "rttbSpatialDoseIndex.h"
#include "rttbExceptionMacros.h"
#include "rttbNullPointerException.h"

namespace rttb
{
  namespace indices
  {

    SpatialDoseIndex::SpatialDoseIndex(core::DoseAccessorInterface::ConstPointer aDose)
      : _dose(aDose)
    {
      if (nullptr == aDose)
      {
        rttbExceptionMacro(core::NullPointerException, "aDose must not be nullptr!");
      }
    }

    const core::GeometricInfo& SpatialDoseIndex::getGeometricInfo() const
    {
      return _dose->getGeometricInfo();
    }
  }
}


