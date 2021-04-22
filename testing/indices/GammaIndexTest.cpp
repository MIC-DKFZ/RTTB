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
      class IndexFacade {
      protected:

          indices::GammaIndex* _index;

      public:
          IndexFacade(
              indices::GammaIndex* index) {
              this->_index = index;
          }
          ~IndexFacade() {}
          /**
           * overload [] function
           */
          DoseTypeGy operator [](int idx) const {
              return _index->getValueAt(idx);
          }
      };

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
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().x());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().y());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().z());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setDistanceToAgreementThreshold(2.);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.03, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().x());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().y());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().z());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setDoseDifferenceThreshold(0.1);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().x());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().y());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().z());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setSearchSamplingRate(3.0,3.0,3.0);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(3.0, gamma.getSearchSamplingRate().x());
      CHECK_EQUAL(3.0, gamma.getSearchSamplingRate().y());
      CHECK_EQUAL(3.0, gamma.getSearchSamplingRate().z());
      CHECK_EQUAL(true, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setSearchSamplingRate(1.0, 1.0, 1.0);
      gamma.setUseLocalDose(false);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().x());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().y());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().z());
      CHECK_EQUAL(false, gamma.getUseLocalDose());
      CHECK_EQUAL(0., gamma.getGlobalDose());

      gamma.setGlobalDose(42.);
      CHECK_EQUAL(2., gamma.getDistanceToAgreementThreshold());
      CHECK_EQUAL(0.1, gamma.getDoseDifferenceThreshold());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().x());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().y());
      CHECK_EQUAL(1.0, gamma.getSearchSamplingRate().z());
      CHECK_EQUAL(false, gamma.getUseLocalDose());
      CHECK_EQUAL(42., gamma.getGlobalDose());
    }

    void Test_Computation()
    {
        PREPARE_DEFAULT_TEST_REPORTING;

        std::cout << "Test values" << std::endl;

        auto nnInterpolator = boost::make_shared<interpolation::LinearInterpolation>();
        auto nnInterpolator2 = boost::make_shared<interpolation::LinearInterpolation>();

        OrientationMatrix testOM(0);
        testOM(0, 0) = 1.0;
        testOM(1, 1) = 1.0;
        testOM(2, 2) = 1.0;

        core::GeometricInfo geoInfo_4_05;
        geoInfo_4_05.setImageSize({ 8,8,1 });
        geoInfo_4_05.setSpacing({ 1.0,1.0,1.0 });
        geoInfo_4_05.setOrientationMatrix(testOM);

        auto doseValues4 = std::vector<DoseTypeGy>({ 4.75, 4.75, 4.75, 4.75,4.75, 4.75, 4.75, 4.75,
                                                    4.80, 4.80, 4.80, 4.80,4.80, 4.80, 4.80, 4.80,
                                                    4.85, 4.85, 4.85, 4.85,4.85, 4.85, 4.85, 4.85,
                                                    4.9 , 4.9 , 4.9 , 4.9 ,4.9 , 4.9 , 4.9 , 4.9 ,
                                                    4.95, 4.95, 4.95, 4.95,4.95, 4.95, 4.95, 4.95,
                                                    5., 5., 5., 5., 5., 5., 5., 5.,
                                                    5.05, 5.05, 5.05, 5.05, 5.05, 5.05, 5.05, 5.05,
                                                    5.1, 5.1, 5.1, 5.1, 5.1, 5.1, 5.1, 5.1 });
        auto doseRefValues4 = std::vector<DoseTypeGy>({ 4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10,
                                                       4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10,
                                                       4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10,
                                                       4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10,
                                                       4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10,
                                                       4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10,
                                                       4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10,
                                                       4.75, 4.8, 4.85, 4.9 , 4.95, 5., 5.05, 5.10 });

        core::DoseAccessorInterface::ConstPointer dose4 = boost::make_shared<DummyDoseAccessor>(doseRefValues4, geoInfo_4_05);
        core::DoseAccessorInterface::ConstPointer refDose4 = boost::make_shared<DummyDoseAccessor>(doseValues4, geoInfo_4_05);
        
        //threshold 2 difference 3 spacing 1
        auto result_2_3_1 = std::vector<DoseTypeGy>({
        0,0.333333,0.600925,0.833333,1,1,1,1,
        0.333333,0,0.333333,0.600925,0.833333,1,1,1,
        0.600925,0.333333,0,0.333333,0.600925,0.833333,1,1,
        0.833333,0.600925,0.333333,0,0.333333,0.600925,0.833333,1,
        1,0.833333,0.600925,0.333333,0,0.333333,0.600925,0.833333,
        1,1,0.833333,0.600925,0.333333,0,0.333333,0.600925,
        1,1,1,0.833333,0.600925,0.333333,0,0.333333,
        1,1,1,1,0.833333,0.600925,0.333333,0
        });

        //threshold 2 difference 3 spacing 05
        auto result_2_3_05 = std::vector<DoseTypeGy>({
        0,0.300463,0.559017,0.833333,1,1,1,1,
        0.300463,0,0.300463,0.559017,0.833333,1,1,1,
        0.559017,0.300463,0,0.300463,0.559017,0.833333,1,1,
        0.833333,0.559017,0.300463,0,0.300463,0.559017,0.833333,1,
        1,0.833333,0.559017,0.300463,0,0.300463,0.559017,0.833333,
        1,1,0.833333,0.559017,0.300463,0,0.300463,0.559017,
        1,1,1,0.833333,0.559017,0.300463,0,0.300463,
        1,1,1,1,0.833333,0.559017,0.300463,0, });

        //threshold 3 difference 3 spacing 1
        auto result_3_3_1 = std::vector<DoseTypeGy>({
        0,0.333333,0.471405,0.745356,0.942809,1,1,1,
        0.333333,0,0.333333,0.471405,0.745356,0.942809,1,1,
        0.471405,0.333333,0,0.333333,0.471405,0.745356,0.942809,1,
        0.745356,0.471405,0.333333,0,0.333333,0.471405,0.745356,0.942809,
        0.942809,0.745356,0.471405,0.333333,0,0.333333,0.471405,0.745356,
        1,0.942809,0.745356,0.471405,0.333333,0,0.333333,0.471405,
        1,1,0.942809,0.745356,0.471405,0.333333,0,0.333333,
        1,1,1,0.942809,0.745356,0.471405,0.333333,0
        });

        indices::GammaIndex gamma1(dose4, refDose4);
        gamma1.setDoseInterpolator(nnInterpolator);
        gamma1.setReferenceDoseInterpolator(nnInterpolator2);
        gamma1.setDistanceToAgreementThreshold(2.0);
        gamma1.setDoseDifferenceThreshold(0.03);
        gamma1.setSearchSamplingRate(1., 1., 1.);
        gamma1.setUseLocalDose(false);
        gamma1.setGlobalDose(5.0);

        indices::GammaIndex gamma2(dose4, refDose4);
        gamma2.setDoseInterpolator(nnInterpolator);
        gamma2.setReferenceDoseInterpolator(nnInterpolator2);
        gamma2.setDistanceToAgreementThreshold(2.0);
        gamma2.setDoseDifferenceThreshold(0.03);
        gamma2.setSearchSamplingRate(.5, .5, .5);
        gamma2.setUseLocalDose(false);
        gamma2.setGlobalDose(5.0);

        indices::GammaIndex gamma3(dose4, refDose4);
        gamma3.setDoseInterpolator(nnInterpolator);
        gamma3.setReferenceDoseInterpolator(nnInterpolator2);
        gamma3.setDistanceToAgreementThreshold(3.0);
        gamma3.setDoseDifferenceThreshold(0.03);
        gamma3.setSearchSamplingRate(1., 1., 1.);
        gamma3.setUseLocalDose(false);
        gamma3.setGlobalDose(5.0);

        std::vector<DoseTypeGy> result1,result2,result3;
        IndexFacade index1(&gamma1);
        IndexFacade index2(&gamma2);
        IndexFacade index3(&gamma3);

        for (int i = 0; i < geoInfo_4_05.getNumberOfVoxels(); i++)
        {
           // std::cout << index[i] << ",";
               result1.push_back(index1[i]);
               result2.push_back(index2[i]);
               result3.push_back(index3[i]);
        }
        CHECK_ARRAY_CLOSE(result_2_3_1, result1, doseRefValues4.size(), 1.0e-5);
        CHECK_ARRAY_CLOSE(result_2_3_05, result2, doseRefValues4.size(),1.0e-5);
        CHECK_ARRAY_CLOSE(result_3_3_1, result3, doseRefValues4.size(), 1.0e-5);
    }

    /*! @brief Test of GammaIndex.*/
    int GammaIndexTest(int /*argc*/, char* /*argv*/[])
    {
      PREPARE_DEFAULT_TEST_REPORTING;

      Test_Initialization();
      Test_Computation();


      RETURN_AND_REPORT_TEST_SUCCESS;
    }

  }//testing
}//rttb