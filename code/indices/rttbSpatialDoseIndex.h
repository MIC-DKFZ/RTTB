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

#ifndef __SPATIAL_DOSE_INDEX_H
#define __SPATIAL_DOSE_INDEX_H

#include "rttbDoseAccessorInterface.h"

#include "RTTBIndicesExports.h"

namespace rttb
{
  namespace indices
  {
    /*! @class SpatialDoseIndex
      @brief This is the base class for indicies that are computed on spatialy resolved
      dose distributions (e.g. gamma index).
      In contrast to other "normal" DVH based indices that are not spatialy resolved (like
      e.g. ConformityIndex), these type of indices are computed on a spatialy resolved dose
      distribution for specifeid points in space. Thus this index type itself has a geometry
      info and is spatialy distributed.
      @ingroup indices
    */
    class RTTBIndices_EXPORT SpatialDoseIndex : core::AccessorInterface
    {
    public:
      rttbClassMacro(SpatialDoseIndex, AccessorInterface);

      const core::GeometricInfo& getGeometricInfo() const override;

    protected:

      /*! @brief Constructor with the referece dose*/
      SpatialDoseIndex(core::DoseAccessorInterface::ConstPointer aDose);
      virtual ~SpatialDoseIndex();

      core::DoseAccessorInterface::ConstPointer _dose;
    };
  }
}


#endif
