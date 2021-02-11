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

#include "rttbGammaIndex.h"
#include "rttbExceptionMacros.h"
#include "rttbNullPointerException.h"
#include "rttbIndexOutOfBoundsException.h"
#include "rttbInvalidDoseException.h"
#include "rttbLinearInterpolation.h"

#include <boost/make_shared.hpp>

namespace rttb
{
  namespace indices
  {
    GammaIndex::DTAPreComputation::DTAPreComputation(WorldCoordinate3D pos, DTAValueType disPen, DTAValueType zeroDoseDiffPen): searchPosition(pos), distancePenalty(disPen), penaltyWithZeroDoseDiff(zeroDoseDiffPen)
    {
    }

    GammaIndex::GammaIndex(core::DoseAccessorInterface::ConstPointer dose,
      core::DoseAccessorInterface::ConstPointer referenceDose) : SpatialDoseIndex(dose), _referenceDose(referenceDose)
    {
      if (nullptr == referenceDose)
      {
        rttbExceptionMacro(core::NullPointerException, << "referenceDose must not be nullptr!");
      }

      _indexGeometry = dose->getGeometricInfo().clone();
      setDoseInterpolator(nullptr);
      setReferenceDoseInterpolator(nullptr);
      this->UpdatePrecomputedDistancePenalties();
    }

    GammaIndex::GammaIndex(core::DoseAccessorInterface::ConstPointer dose,
      core::DoseAccessorInterface::ConstPointer referenceDose,
      core::GeometricInfo indexGeometry) : SpatialDoseIndex(dose), _referenceDose(referenceDose)
    {
      if (nullptr == referenceDose)
      {
        rttbExceptionMacro(core::NullPointerException, << "referenceDose must not be nullptr!");
      }

      _indexGeometry = indexGeometry.clone();
      setDoseInterpolator(nullptr);
      setReferenceDoseInterpolator(nullptr);
      this->UpdatePrecomputedDistancePenalties();
    }

    const core::GeometricInfo& GammaIndex::getGeometricInfo() const
    {
      return *_indexGeometry;
    }

    GenericValueType GammaIndex::getValueAt(const VoxelGridID aID) const
    {
      VoxelGridIndex3D gridIndex;
      if (!_indexGeometry->convert(aID, gridIndex))
      {
        rttbExceptionMacro(core::IndexOutOfBoundsException, << "Cannot get gamma index by grid ID. ID is not valid for geometric info of the gamma index. Invalid ID: " << aID);
      }
      return this->getValueAt(gridIndex);
    }

    GenericValueType GammaIndex::getValueAt(const VoxelGridIndex3D& aIndex) const
    {
      WorldCoordinate3D aPoint;
      if (!_indexGeometry->indexToWorldCoordinate(aIndex, aPoint))
      {
        rttbExceptionMacro(core::IndexOutOfBoundsException, << "Cannot get gamma index by grid index. Grid index is not valid for geometric info of the gamma index. Invalid grid index: " << aIndex);
      }
      return computeValueAndPosition(aPoint).first;
    }

    GenericValueType GammaIndex::getValueAt(const WorldCoordinate3D& aPoint) const
    {
      if (!_indexGeometry->isInside(aPoint))
      {
        rttbExceptionMacro(core::IndexOutOfBoundsException, << "Cannot get gamma index by point. Point is not valid for geometric info of the gamma index. Invalid point: " << aPoint);
      }

      return computeValueAndPosition(aPoint).first;
    }

    const IDType GammaIndex::getUID() const
    {
      std::stringstream uidStream;
      uidStream << "gammaindex." << _dta << "." << _searchSamplingRate << "." << _ddt << "." << _useLocalDose << "." << _globalDose << "_" << _dose->getUID() << "_" << _referenceDose->getUID();
      return uidStream.str();
    }

    void GammaIndex::setDoseInterpolator(interpolation::InterpolationBase::Pointer interpolator)
    {
      if (nullptr == interpolator)
      {
        _doseInterpolator = ::boost::make_shared<interpolation::LinearInterpolation>();
      }
      else
      {
        _doseInterpolator = interpolator;
      }
      _doseInterpolator->setAccessorPointer(_dose);
    }

    interpolation::InterpolationBase::ConstPointer GammaIndex::getDoseInterpolator() const
    {
      return _doseInterpolator;
    };

    void GammaIndex::setReferenceDoseInterpolator(interpolation::InterpolationBase::Pointer interpolator)
    {
      if (nullptr == interpolator)
      {
        _referenceDoseInterpolator = ::boost::make_shared<interpolation::LinearInterpolation>();
      }
      else
      {
        _referenceDoseInterpolator = interpolator;
      }
      _referenceDoseInterpolator->setAccessorPointer(_referenceDose);
    }

    interpolation::InterpolationBase::ConstPointer GammaIndex::getReferenceDoseInterpolator() const
    {
      return _referenceDoseInterpolator;
    }

    void GammaIndex::setDistanceToAgreementThreshold(DTAValueType dta)
    {
      if (dta != _dta)
      {
        _dta = dta;
        this->UpdatePrecomputedDistancePenalties();
      }
    }

    GammaIndex::DTAValueType GammaIndex::getDistanceToAgreementThreshold() const
    {
      return _dta;
    }

    void GammaIndex::setDoseDifferenceThreshold(DDTValueType ddt)
    {
       _ddt = ddt;
    }

    GammaIndex::DDTValueType GammaIndex::getDoseDifferenceThreshold() const
    {
      return _ddt;
    }

    void GammaIndex::setSearchSamplingRate(unsigned int rate)
    {
      if (rate != _searchSamplingRate)
      {
        _searchSamplingRate = rate;
        this->UpdatePrecomputedDistancePenalties();
      }
    }

    unsigned int GammaIndex::getSearchSamplingRate() const
    {
      return _searchSamplingRate;
    }

    void GammaIndex::setUseLocalDose(bool useLocalDose)
    {
      _useLocalDose = useLocalDose;
    }

    bool GammaIndex::getUseLocalDose() const
    {
      return _useLocalDose;
    }

    void GammaIndex::setGlobalDose(DoseTypeGy globalDose)
    {
      _globalDose = globalDose;
    }

    DoseTypeGy GammaIndex::getGlobalDose() const
    {
      return _globalDose;
    }

    void GammaIndex::UpdatePrecomputedDistancePenalties()
    {

      DATPreComputationVectorType newPenalties;
      const auto min = -1 * static_cast<int>(_searchSamplingRate);
      const auto max = static_cast<int>(_searchSamplingRate);

      /*We add the penalty for the search center (measured point)
        explicitly at the beginning to check it first. This allows
        us to shortcut in cases where the measured point equals the
        expected or is very close.*/
      newPenalties.emplace_back(WorldCoordinate3D( 0.,0.,0. ), 0., 0.);

      for (int iX = min; iX <= max; ++iX)
      {
        const WorldCoordinate x = _dta * iX / static_cast<double>(_searchSamplingRate);
        for (int iY = min; iY <= max; ++iY)
        {
          const WorldCoordinate y = _dta * iY / static_cast<double>(_searchSamplingRate);
          for (int iZ = min; iZ <= max; ++iZ)
          {
            const WorldCoordinate z = _dta * iZ / static_cast<double>(_searchSamplingRate);
            WorldCoordinate3D newPos = { x,y,z };
            const auto newDistance = boost::numeric::ublas::norm_2(newPos);
            const auto penalty = (newDistance * newDistance) / (_dta * _dta);
            if (penalty>0 && penalty <= 1)
            { //we skip the origin (penalty == 0) as it was added before the loop
              //and we skip every point that would not pass because the dta penalty is to high (>1)
              newPenalties.emplace_back(newPos, penalty, std::sqrt(penalty));
            }
          }
        }
      }
      _precomputedDistancePenalties = newPenalties;
    }

    std::pair<GenericValueType, WorldCoordinate3D> GammaIndex::computeValueAndPosition(const WorldCoordinate3D& aPoint) const
    {
      const auto measuredDose = _doseInterpolator->getValue(aPoint);
      const auto referenceDose = _referenceDoseInterpolator->getValue(aPoint);

      const DoseTypeGy doseThresholdGy = ((_useLocalDose) ? referenceDose : _globalDose) * _ddt;
      const DoseTypeGy doseThresholdGySquared = doseThresholdGy*doseThresholdGy;

      if (0. == doseThresholdGySquared)
      {
        //TODO needs to clarify what to do if dose threshold is 0 because we would devide through it?!??
        return std::make_pair(std::nan(""),WorldCoordinate3D(0.));
      }

      std::pair<GenericValueType, WorldCoordinate3D> bestFinding;
      bestFinding.second = WorldCoordinate3D(std::numeric_limits<WorldCoordinate>::max());
      bestFinding.first = std::numeric_limits<GenericValueType>::max();

      for (const auto& distancePenalty : _precomputedDistancePenalties)
      {
        if (distancePenalty.penaltyWithZeroDoseDiff < bestFinding.first)
        { //search position could beat the best finding -> evaluate it
          const WorldCoordinate3D refPoint = aPoint + distancePenalty.searchPosition;

          if (_indexGeometry->isInside(refPoint) && _referenceDose->getGeometricInfo().isInside(refPoint))
          { //needed refpoint is part of index and reference dose geometry -> go on
            const auto refDose = _referenceDoseInterpolator->getValue(refPoint);
            const auto doseDifferenceSquared = std::pow(refDose - measuredDose, 2);
            const auto dosePenalty = doseDifferenceSquared / doseThresholdGySquared;
            const GenericValueType penalty = std::sqrt(distancePenalty.distancePenalty + dosePenalty);
            if (penalty < bestFinding.first)
            {
                //gamma index value is limited to 1.0 based on the literature
                bestFinding.first = std::min(penalty, 1.0);
                bestFinding.second = distancePenalty.searchPosition;
            }
          }
        }
      }
      return bestFinding;
    }

  }
}


