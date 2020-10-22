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

#ifndef __GAMMA_INDEX_H
#define __GAMMA_INDEX_H

#include "rttbSpatialDoseIndex.h"

#include "rttbInterpolationBase.h"

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
    class RTTBIndices_EXPORT GammaIndex : SpatialDoseIndex
    {
    public:
      rttbClassMacro(GammaIndex, SpatialDoseIndex);

      /*! @brief Constructor with the referece dose*/
      GammaIndex(core::DoseAccessorInterface::ConstPointer computedDose,
                 core::DoseAccessorInterface::ConstPointer referenceDose);

      GammaIndex(core::DoseAccessorInterface::ConstPointer dose,
                 core::DoseAccessorInterface::ConstPointer referenceDose,
                 double distaneToAgreementThreshold, double doseDifferenceThreshold);
      virtual ~GammaIndex();

      GenericValueType getValueAt(const VoxelGridID aID) const override;

      GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const override;

      const IDType getUID() const override;

    protected:

      /** Reference dose used to compare the computed dose with.*/
      core::DoseAccessorInterface::ConstPointer _referenceDose;

      interpolation::InterpolationBase::Pointer _doseInterpolator;
      interpolation::InterpolationBase::Pointer _referenceDoseInterpolator;

      /** Distance to aggreement (DTA) threshold for the gamma index comutation. Specified in mm.*/
      double _dtaThreshold = 3.;

      /** To calculate the gamma index one has to sample the reference dose within the DTA. This
       variable controls how dense the sampling is within the Dit is sampled a Distance to aggreement (DTA) threshold for the gamma index comutation. Specified in mm.*/
      unsigned int _spatialResolution = 10;

      /** in %*/
      double _doseDifferenceThreshold = 3.;

      /** */
      bool _useLocalDose = false;

      DoseTypeGy _globalDose = 0.;

    };
  }
}


#endif
