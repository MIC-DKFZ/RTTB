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

#include <map>

#include "rttbSpatialDoseIndex.h"
#include "rttbInterpolationBase.h"

#include "RTTBIndicesExports.h"

namespace rttb
{
  namespace indices
  {
    /*! @class GammaIndex
      @brief TODO
      @ingroup indices
    */
    class RTTBIndices_EXPORT GammaIndex : SpatialDoseIndex
    {
    public:
      rttbClassMacro(GammaIndex, SpatialDoseIndex);

      /**Type used to specify the distance to agreement in mm.*/
      using DTAValueType = double;
      /**Type used to specify the dose difference threshold in fractions (0= 0% and 0.5=50%).*/
      using DDTValueType = double;


      /** Constructor that uses takes the dose and reference dose to compute the
      gamma index. As geometric info (spatial resolution) of the index the geometric
      info of dose will be used.
      @pre dose must point to a valid instance.
      @pre referenceDose must point to a valid instance.*/
      GammaIndex(core::DoseAccessorInterface::ConstPointer dose,
        core::DoseAccessorInterface::ConstPointer referenceDose);

      /** Constructor that allows to explicitly allows to specify the geometric info
       of the index (e.g. if you want to have the index in another resolution.
       @pre dose must point to a valid instance.
       @pre referenceDose must point to a valid instance.*/
      GammaIndex(core::DoseAccessorInterface::ConstPointer dose,
        core::DoseAccessorInterface::ConstPointer referenceDose,
        core::GeometricInfo indexGeometry);

      virtual ~GammaIndex() = default;

      GenericValueType getValueAt(const VoxelGridID aID) const override;

      GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const override;

      GenericValueType getValueAt(const WorldCoordinate3D& aPoint) const;

      const IDType getUID() const override;

      const core::GeometricInfo& getGeometricInfo() const override;

      /**Allows to specify the interpolator that should be used if dose should be sampled.
        If no interpolator is defined (nullptr), a LinarInterpolator will be used
        (this is also the default after index construction).
        Remark: After setting the interpolator, the index assumes that it takes control over
        the interpolator and set the input accessor. Please do not change the state of the
        interpolator in the lifetime of the index instance that took over control. Otherwise
        strange side effects could happen in the computation.*/
      void setDoseInterpolator(interpolation::InterpolationBase::Pointer interpolator);
      interpolation::InterpolationBase::ConstPointer getDoseInterpolator() const;

      /**Allows to specify the interpolator that should be used if referenceDose should be sampled.
        If no interpolator is defined (nullptr), a LinarInterpolator will be used
        (this is also the default after index construction).
        Remark: After setting the interpolator, the index assumes that it takes control over
        the interpolator and set the input accessor. Please do not change the state of the
        interpolator in the lifetime of the index instance that took over control. Otherwise
        strange side effects could happen in the computation.*/
      void setReferenceDoseInterpolator(interpolation::InterpolationBase::Pointer interpolator);
      interpolation::InterpolationBase::ConstPointer getReferenceDoseInterpolator() const;

      void setDistanceToAgreementThreshold(DTAValueType dta);
      DTAValueType getDistanceToAgreementThreshold() const;

      void setDoseDifferenceThreshold(DDTValueType ddt);
      DDTValueType getDoseDifferenceThreshold() const;

      void setSearchSamplingRate(unsigned int rate);
      unsigned int getSearchSamplingRate() const;

      void setUseLocalDose(bool useLocalDose);
      bool getUseLocalDose() const;

      void setGlobalDose(DoseTypeGy globalDose);
      DoseTypeGy getGlobalDose() const;

    protected:

      /** GeometricInfo that should be used for the index. Either the geometric info
      of the dose or a user defined geometric info.*/
      core::GeometricInfo::ConstPointer _indexGeometry;

      /** Reference dose used to compare the computed dose with.*/
      core::DoseAccessorInterface::ConstPointer _referenceDose;

      /** Interpolator used when the index needs to access values of _dose*/
      interpolation::InterpolationBase::Pointer _doseInterpolator;
      /** Interpolator used when the index needs to access values of _referenceDose*/
      interpolation::InterpolationBase::Pointer _referenceDoseInterpolator;

      /** Distance to aggreement (DTA) threshold for the gamma index comutation. Specified in mm.
       It also specified the search range radius (in mm) arround the the center point (world coordinates)
       requested for the index value.*/
      DTAValueType _dta = 3.;

      /** dose difference as fraction (0= 0% and 0.5=50%) that is the threshold*/
      DDTValueType _ddt = 0.03;

      /** To calculate the gamma index one has to sample the reference dose within the DTA. This
       variable controls how dense the sampling is within the Dit is sampled a Distance to aggreement (DTA) threshold for the gamma index comutation. Specified in mm.*/
      unsigned int _searchSamplingRate = 5;

      /** */
      bool _useLocalDose = true;

      DoseTypeGy _globalDose = 0.;

      /** Internal helper that stores a search position vector that has been
       computed given the specified distance to aggreement and the search sampling rate.*/
      struct DTAPreComputation
      {
        /** The search position is relative to the "measured" point for which
        the gamma index should be computed. Thus search position(0, 0, 0) is
        the "measured" point itself.*/
        WorldCoordinate3D searchPosition;

        /** The precomputed distance penalty part of the penalty formular.*/
        DTAValueType distancePenalty = 0.0;

        /** The complete penalty for the seach position, assuming that the dose
        difference penalty is 0. This the sqrt(distancePenalty).*/
        DTAValueType penaltyWithZeroDoseDiff = 0.0;

        DTAPreComputation(WorldCoordinate3D pos, DTAValueType disPen, DTAValueType zeroDoseDiffPen);
        DTAPreComputation() = default;
      };

      using DATPreComputationVectorType = std::vector< DTAPreComputation >;
      /** Collection of all precomputed search positions and there penalties.*/
      DATPreComputationVectorType _precomputedDistancePenalties;

      /** function can be called to update the _precomputedDistancePenalties given the current
       index settings. After the update only search position are in _precomputedDistancePenalties,
       that do not fail the DTA.\n
       The implementation makes use of the fact that the distance penalty
       part of the gamma index does not depend on the dose distribution itself, and can be
       precomputed for any dose distribution and any search position as soon as DTA and sampling rate
       is defined.*/
      void UpdatePrecomputedDistancePenalties();

      std::pair<GenericValueType,WorldCoordinate3D> computeValueAndPosition(const WorldCoordinate3D& aPoint) const;
    };
  }
}


#endif
