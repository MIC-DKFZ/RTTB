// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include "litCheckMacros.h"
#include "rttbDoseIndex.h"
#include "rttbBaseType.h"
#include "rttbNullPointerException.h"
#include "rttbGammaIndex.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"
#include "rttbNullPointerException.h"
#include "rttbLinearInterpolation.h"
#include "rttbNearestNeighborInterpolation.h"

#include "DummyDoseAccessor.h"

#include <math.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

namespace rttb
{
  namespace testing
  {
    void Test_Initialization()
    {
      PREPARE_DEFAULT_TEST_REPORTING;


      core::DoseAccessorInterface::ConstPointer invalidDose;
      core::DoseAccessorInterface::ConstPointer simpleDose = boost::make_shared<DummyDoseAccessor>();

      CHECK_THROW_EXPLICIT(indices::GammaIndex(invalidDose, invalidDose),
        core::NullPointerException);
      CHECK_THROW_EXPLICIT(indices::GammaIndex(invalidDose, invalidDose,core::GeometricInfo()),
        core::NullPointerException);
      CHECK_THROW_EXPLICIT(indices::GammaIndex(simpleDose, invalidDose),
        core::NullPointerException);
      CHECK_THROW_EXPLICIT(indices::GammaIndex(simpleDose, invalidDose, core::GeometricInfo()),
        core::NullPointerException);
      CHECK_THROW_EXPLICIT(indices::GammaIndex(invalidDose, simpleDose),
        core::NullPointerException);
      CHECK_THROW_EXPLICIT(indices::GammaIndex(invalidDose, simpleDose, core::GeometricInfo()),
        core::NullPointerException);

      core::GeometricInfo geoInfo;
      geoInfo.setImageSize({ 2,2,2 });
      geoInfo.setSpacing({ 1,1,1 });

      core::GeometricInfo geoInfo2;
      geoInfo2.setImageSize({ 3,3,3 });
      geoInfo2.setSpacing({ 1,1,1 });

      core::DoseAccessorInterface::ConstPointer refDose = boost::make_shared<DummyDoseAccessor>(std::vector<DoseTypeGy>({ 1,2,3,4,5,6,7,8 }), geoInfo);

      indices::GammaIndex gamma(simpleDose, refDose);
      indices::GammaIndex gamma2(simpleDose, refDose, geoInfo2);

      CHECK_EQUAL(simpleDose->getGeometricInfo(), gamma.getGeometricInfo());
      CHECK_EQUAL(geoInfo2, gamma2.getGeometricInfo());

      //CHECK interpolator setting
      CHECK(nullptr != dynamic_cast<const interpolation::LinearInterpolation*>(gamma.getDoseInterpolator().get()));
      CHECK(gamma.getDoseInterpolator()->getAccessorPointer() == simpleDose);
      CHECK(nullptr != dynamic_cast<const interpolation::LinearInterpolation*>(gamma.getReferenceDoseInterpolator().get()));
      CHECK(gamma.getReferenceDoseInterpolator()->getAccessorPointer() == refDose);

      auto nnInterpolator = boost::make_shared<interpolation::NearestNeighborInterpolation>();
      auto nnInterpolator2 = boost::make_shared<interpolation::NearestNeighborInterpolation>();

      gamma.setDoseInterpolator(nnInterpolator);
      CHECK(nnInterpolator == gamma.getDoseInterpolator());
      CHECK(nnInterpolator->getAccessorPointer() == simpleDose);
      CHECK(nullptr != dynamic_cast<const interpolation::LinearInterpolation*>(gamma.getReferenceDoseInterpolator().get()));

      gamma.setReferenceDoseInterpolator(nnInterpolator2);
      CHECK(nnInterpolator == gamma.getDoseInterpolator());
      CHECK(nnInterpolator->getAccessorPointer() == simpleDose);
      CHECK(nnInterpolator2 == gamma.getReferenceDoseInterpolator());
      CHECK(nnInterpolator2->getAccessorPointer() == refDose);

      gamma.setDoseInterpolator(nullptr);
      CHECK(nullptr != dynamic_cast<const interpolation::LinearInterpolation*>(gamma.getDoseInterpolator().get()));
      CHECK(gamma.getDoseInterpolator()->getAccessorPointer() == simpleDose);
      CHECK(nnInterpolator2 == gamma.getReferenceDoseInterpolator());
      CHECK(nnInterpolator2->getAccessorPointer() == refDose);

      gamma.setReferenceDoseInterpolator(nullptr);
      CHECK(nullptr != dynamic_cast<const interpolation::LinearInterpolation*>(gamma.getDoseInterpolator().get()));
      CHECK(gamma.getDoseInterpolator()->getAccessorPointer() == simpleDose);
      CHECK(nullptr != dynamic_cast<const interpolation::LinearInterpolation*>(gamma.getReferenceDoseInterpolator().get()));
      CHECK(gamma.getReferenceDoseInterpolator()->getAccessorPointer() == refDose);

      //CHECK other settings
      CHECK_EQUAL(3., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.03, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(5u, gamma.getSearchSamplingRate());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setDistanceToAgreementThreshold(2.);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.03, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(5u, gamma.getSearchSamplingRate());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setDoseDifferenceThreshold(0.1);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(5u, gamma.getSearchSamplingRate());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setSearchSamplingRate(3);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(3u, gamma.getSearchSamplingRate());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setUseLocalDose(false);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(3u, gamma.getSearchSamplingRate());
      CHECK_EQUAL(false, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setGlobalDose(42.);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(3u, gamma.getSearchSamplingRate());
      CHECK_EQUAL(false, gamma.getUseLocalDose());
      CHECK_EQUAL(42., gamma.getGlobalDose());
    }

    /*! @brief Test of GammaIndex.*/
    int GammaIndexTest(int /*argc*/, char* /*argv*/[])
    {
      PREPARE_DEFAULT_TEST_REPORTING;

      Test_Initialization();


      RETURN_AND_REPORT_TEST_SUCCESS;
    }

  }//testing
}//rttb