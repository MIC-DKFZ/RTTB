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

#include "litCheckMacros.h"

#include "rttbBaseType.h"

namespace rttb
{
	namespace testing
	{

		/*! @brief BaseTypeTests - tests the API for the classes in baseType
		1) UnsignedIndex3D
    2) WorldCoordinate3D
    3) SpacingVectorType3D
    4) OrientationMatrix 
    5) VoxelGridIndex3D
    6) VoxelGridIndex2D
		*/
		int BaseTypeTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

      //1)  UnsignedIndex3D
      CHECK_NO_THROW(UnsignedIndex3D ui);
      UnsignedIndex3D emptyUnsignedIndex3D;
      CHECK_EQUAL(emptyUnsignedIndex3D.x(), 0);
      CHECK_EQUAL(emptyUnsignedIndex3D.y(), 0);
      CHECK_EQUAL(emptyUnsignedIndex3D.z(), 0);

      CHECK_NO_THROW(UnsignedIndex3D ui(5));
      UnsignedIndex3D sameValueUnsignedIndex(5);
      CHECK_EQUAL(sameValueUnsignedIndex.x(), 5);
      CHECK_EQUAL(sameValueUnsignedIndex.y(), 5);
      CHECK_EQUAL(sameValueUnsignedIndex.z(), 5);

      CHECK_NO_THROW(UnsignedIndex3D ui(5, 8, 42));
      UnsignedIndex3D differentValueUnsignedIndex(5, 8, 42);
      CHECK_EQUAL(differentValueUnsignedIndex.x(), 5);
      CHECK_EQUAL(differentValueUnsignedIndex.y(), 8);
      CHECK_EQUAL(differentValueUnsignedIndex.z(), 42);

      UnsignedIndex3D threeDimensionalUnsignedIndexSame(5, 8, 42);
      UnsignedIndex3D threeDimensionalUnsignedIndexDifferent(1, 2, 3);

      CHECK(differentValueUnsignedIndex == threeDimensionalUnsignedIndexSame);
      CHECK_EQUAL(differentValueUnsignedIndex == threeDimensionalUnsignedIndexDifferent, false);
      CHECK_EQUAL(differentValueUnsignedIndex == sameValueUnsignedIndex, false);
      CHECK_EQUAL(emptyUnsignedIndex3D == sameValueUnsignedIndex, false);

      //2) WorldCoordinate3D
      CHECK_NO_THROW(WorldCoordinate3D wc);
      WorldCoordinate3D emptyWC3D;
      CHECK_EQUAL(emptyWC3D.x(), 0);
      CHECK_EQUAL(emptyWC3D.y(), 0);
      CHECK_EQUAL(emptyWC3D.z(), 0);

      CHECK_NO_THROW(WorldCoordinate3D wc(6.5));
      WorldCoordinate3D sameValueWC3D(6.5);
      CHECK_EQUAL(sameValueWC3D.x(), 6.5);
      CHECK_EQUAL(sameValueWC3D.y(), 6.5);
      CHECK_EQUAL(sameValueWC3D.z(), 6.5);

      CHECK_NO_THROW(WorldCoordinate3D(5.8, 0.1, -2.7));
      WorldCoordinate3D differentValueWC3D(5.8, 0.1, -2.7);
      CHECK_EQUAL(differentValueWC3D.x(), 5.8);
      CHECK_EQUAL(differentValueWC3D.y(), 0.1);
      CHECK_EQUAL(differentValueWC3D.z(), -2.7);

      CHECK_EQUAL(differentValueWC3D.toString(), "5.800000 0.100000 -2.700000");

      WorldCoordinate3D differentValueWC3Dsecond(1.5, -3.9, 0.7);
      WorldCoordinate3D resultWC3DCrossTrue(-10.46, -8.11, -22.77);
      CHECK_NO_THROW(differentValueWC3D.cross(differentValueWC3Dsecond));
      auto resultWC3DCrossComputed = differentValueWC3D.cross(differentValueWC3Dsecond);
      CHECK_CLOSE(resultWC3DCrossComputed.x(), resultWC3DCrossTrue.x(), errorConstant);
      CHECK_CLOSE(resultWC3DCrossComputed.y(), resultWC3DCrossTrue.y(), errorConstant);
      CHECK_CLOSE(resultWC3DCrossComputed.z(), resultWC3DCrossTrue.z(), errorConstant);
      CHECK_NO_THROW(differentValueWC3Dsecond.cross(differentValueWC3D));
      auto resultWC3DCrossComputedOrder = differentValueWC3Dsecond.cross(differentValueWC3D);
      CHECK_CLOSE(resultWC3DCrossComputed.x(), resultWC3DCrossComputedOrder.x()*-1, errorConstant);
      CHECK_CLOSE(resultWC3DCrossComputed.y(), resultWC3DCrossComputedOrder.y()*-1, errorConstant);
      CHECK_CLOSE(resultWC3DCrossComputed.z(), resultWC3DCrossComputedOrder.z()*-1, errorConstant);

      differentValueWC3D = differentValueWC3Dsecond;
      CHECK_EQUAL(differentValueWC3D.x(), 1.5);
      CHECK_EQUAL(differentValueWC3D.y(), -3.9);
      CHECK_EQUAL(differentValueWC3D.z(), 0.7);

      boost::numeric::ublas::vector<WorldCoordinate> wcUblas(3, 4.5);
      differentValueWC3D = wcUblas;
      CHECK_EQUAL(differentValueWC3D.x(), 4.5);
      CHECK_EQUAL(differentValueWC3D.y(), 4.5);
      CHECK_EQUAL(differentValueWC3D.z(), 4.5);

      WorldCoordinate3D WCresultMinus;
      CHECK_NO_THROW(WCresultMinus = differentValueWC3D- differentValueWC3Dsecond);
      CHECK_EQUAL(WCresultMinus.x(), differentValueWC3D.x()- differentValueWC3Dsecond.x());
      CHECK_EQUAL(WCresultMinus.y(), differentValueWC3D.y() - differentValueWC3Dsecond.y());
      CHECK_EQUAL(WCresultMinus.z(), differentValueWC3D.z() - differentValueWC3Dsecond.z());

      WorldCoordinate3D WCresultPlus;
      CHECK_NO_THROW(WCresultPlus = differentValueWC3D + differentValueWC3Dsecond);
      CHECK_EQUAL(WCresultPlus.x(), differentValueWC3D.x() + differentValueWC3Dsecond.x());
      CHECK_EQUAL(WCresultPlus.y(), differentValueWC3D.y() + differentValueWC3Dsecond.y());
      CHECK_EQUAL(WCresultPlus.z(), differentValueWC3D.z() + differentValueWC3Dsecond.z());

      WorldCoordinate3D sameAsWcUblas(4.5);
      CHECK_EQUAL(resultWC3DCrossTrue== differentValueWC3Dsecond, false);
      CHECK_EQUAL(differentValueWC3D == sameAsWcUblas, true);

      WorldCoordinate3D sameAsWcUblasAlmost(4.5+1e-6, 4.5-1e-6, 4.5+2e-7);
      CHECK_EQUAL(sameAsWcUblas.equalsAlmost(sameAsWcUblasAlmost), true);
      CHECK_EQUAL(sameAsWcUblas.equalsAlmost(resultWC3DCrossComputedOrder), false);

      //3) SpacingVectorType
      CHECK_NO_THROW(SpacingVectorType3D svt);
      SpacingVectorType3D emptySvt;
      CHECK_EQUAL(emptySvt.x(), 0);
      CHECK_EQUAL(emptySvt.y(), 0);
      CHECK_EQUAL(emptySvt.z(), 0);

      CHECK_NO_THROW(SpacingVectorType3D svt(1.5));
	  CHECK_THROW(SpacingVectorType3D svt(-1.5));
      SpacingVectorType3D sameValueSvt(1.5);
      CHECK_EQUAL(sameValueSvt.x(), 1.5);
      CHECK_EQUAL(sameValueSvt.y(), 1.5);
      CHECK_EQUAL(sameValueSvt.z(), 1.5);

      CHECK_NO_THROW(SpacingVectorType3D svt(1.5, 1.5, 0.5));
      CHECK_THROW(SpacingVectorType3D svt(1.5, -1.5, 0.5));
      CHECK_THROW(SpacingVectorType3D svt(-1.5, -1.5, -0.5));
      SpacingVectorType3D differentValuesSvt(1.5, 1.5, 0.5);
      CHECK_EQUAL(differentValuesSvt.x(), 1.5);
      CHECK_EQUAL(differentValuesSvt.y(), 1.5);
      CHECK_EQUAL(differentValuesSvt.z(), 0.5);

      SpacingVectorType3D differentValuesSvtChanged(0.5, 0.5, 1.5);
      CHECK_NO_THROW(SpacingVectorType3D svt(differentValuesSvtChanged));
      SpacingVectorType3D svtNew(differentValuesSvtChanged);
      CHECK_EQUAL(svtNew.x(), 0.5);
      CHECK_EQUAL(svtNew.y(), 0.5);
      CHECK_EQUAL(svtNew.z(), 1.5);

      CHECK_NO_THROW(svtNew = differentValuesSvt);
      CHECK_EQUAL(svtNew.x(), 1.5);
      CHECK_EQUAL(svtNew.y(), 1.5);
      CHECK_EQUAL(svtNew.z(), 0.5);

      CHECK_NO_THROW(svtNew = differentValueWC3D);
      CHECK_EQUAL(svtNew.x(), 4.5);
      CHECK_EQUAL(svtNew.y(), 4.5);
      CHECK_EQUAL(svtNew.z(), 4.5);

      boost::numeric::ublas::vector<GridVolumeType> ublasVector(3, 0.5);
      CHECK_NO_THROW(svtNew = ublasVector);
      CHECK_EQUAL(svtNew.x(), 0.5);
      CHECK_EQUAL(svtNew.y(), 0.5);
      CHECK_EQUAL(svtNew.z(), 0.5);

      SpacingVectorType3D sameAsUblasVector(0.5);

      CHECK_EQUAL(svtNew== differentValuesSvtChanged, false);
      CHECK_EQUAL(svtNew == sameAsUblasVector, true);

      SpacingVectorType3D almostSameAsUblasVector(0.5+1e-6, 0.5-1e-6,0.5+2e-7);
      CHECK(svtNew.equalsAlmost(almostSameAsUblasVector));

      CHECK_EQUAL(differentValuesSvtChanged.toString(), "0.500000 0.500000 1.500000");

      //4) OrientationMatrix
      CHECK_NO_THROW(OrientationMatrix om);
      OrientationMatrix om;
      CHECK_EQUAL(om(0, 0), 1.0);
      CHECK_EQUAL(om(1, 1), 1.0);
      CHECK_EQUAL(om(2, 2), 1.0);
      CHECK_EQUAL(om(0, 1), 0.0);
      CHECK_EQUAL(om(0, 2), 0.0);
      CHECK_EQUAL(om(1, 0), 0.0);
      CHECK_EQUAL(om(1, 2), 0.0);
      CHECK_EQUAL(om(2, 0), 0.0);
      CHECK_EQUAL(om(2, 1), 0.0);

      CHECK_NO_THROW(OrientationMatrix omValue(0.1));
      OrientationMatrix omValue(0.1);
      for (unsigned int i = 0; i < 3; i++) {
        for (unsigned int j = 0; j < 3; j++) {
          CHECK_EQUAL(omValue(i, j), 0.1);
        }
      }

      OrientationMatrix omValueAlmost(0.1+1e-6);
      CHECK_EQUAL(omValue.equalsAlmost(omValueAlmost), true);
      CHECK_EQUAL(omValue.equalsAlmost(om), false);

      OrientationMatrix omSame;
      CHECK_EQUAL(om == omSame, true);
      CHECK_EQUAL(omValue == omValueAlmost, false);

      //5) VoxelGridIndex3D
      CHECK_NO_THROW(VoxelGridIndex3D vgi);
      VoxelGridIndex3D vgi;
      CHECK_EQUAL(vgi.x(), 0);
      CHECK_EQUAL(vgi.y(), 0);
      CHECK_EQUAL(vgi.z(), 0);

      CHECK_NO_THROW(VoxelGridIndex3D vgiValue(2));
      VoxelGridIndex3D vgiValue(2);
      CHECK_EQUAL(vgiValue.x(), 2);
      CHECK_EQUAL(vgiValue.y(), 2);
      CHECK_EQUAL(vgiValue.z(), 2);

      CHECK_NO_THROW(VoxelGridIndex3D vgiValueDifferent(2,3,5));
      VoxelGridIndex3D vgiValueDifferent(2,3,5);
      CHECK_EQUAL(vgiValueDifferent.x(), 2);
      CHECK_EQUAL(vgiValueDifferent.y(), 3);
      CHECK_EQUAL(vgiValueDifferent.z(), 5);

      CHECK_EQUAL(vgiValueDifferent.toString(), "2 3 5");

      CHECK_NO_THROW(vgiValueDifferent = threeDimensionalUnsignedIndexSame);
      CHECK_EQUAL(vgiValueDifferent.x(), 5);
      CHECK_EQUAL(vgiValueDifferent.y(), 8);
      CHECK_EQUAL(vgiValueDifferent.z(), 42);

      VoxelGridIndex3D vgiValueDifferentSame(5, 8, 42);

      CHECK_EQUAL(vgi==vgiValue, false);
      CHECK_EQUAL(vgiValueDifferentSame == vgiValueDifferent, true);

      //6) VoxelGridIndex2D
      CHECK_NO_THROW(VoxelGridIndex2D vgi);
      VoxelGridIndex2D vgi2D;
      CHECK_EQUAL(vgi2D.x(), 0);
      CHECK_EQUAL(vgi2D.y(), 0);

      CHECK_NO_THROW(VoxelGridIndex2D vgi2DValue(2));
      VoxelGridIndex2D vgi2DValue(2);
      CHECK_EQUAL(vgi2DValue.x(), 2);
      CHECK_EQUAL(vgi2DValue.y(), 2);

      CHECK_NO_THROW(VoxelGridIndex2D vgi2DValueDifferent(2, 3));
      VoxelGridIndex2D vgi2DValueDifferent(2, 3);
      CHECK_EQUAL(vgi2DValueDifferent.x(), 2);
      CHECK_EQUAL(vgi2DValueDifferent.y(), 3);

      CHECK_EQUAL(vgi2DValueDifferent.toString(), "2 3");

      VoxelGridIndex2D vgi2DValueDifferentSame(2, 3);

      CHECK_EQUAL(vgi2D == vgi2DValueDifferent, false);
      CHECK_EQUAL(vgi2DValueDifferent == vgi2DValueDifferentSame, true);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb