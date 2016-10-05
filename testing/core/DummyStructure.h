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

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests


#include "rttbStructure.h"
#include "CreateTestStructure.h"
#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"

namespace rttb
{
	namespace testing
	{

		/*! @class DummyStructure
			@brief generate simple geometric testing structures.
			The maximal x coordinate used is 9 and the maximal y coordinate is 8.
			Make sure the geometricInfo corresponds to a sufficiently large data grid.
			@see CreateTestStructures
		*/
		class DummyStructure
		{

		private:
			core::GeometricInfo _geoInfo;

		public:
			~DummyStructure();

			DummyStructure(const core::GeometricInfo& aGeoInfo);

			const core::GeometricInfo& getGeometricInfo()
			{
				return _geoInfo;
			};

			core::Structure CreateRectangularStructureCentered(GridIndexType zPlane);

            /* Generate rectangular structure for the z slice and another slice between z and z+1. So the structure has a smaller z spacing than the dose
            */
            core::Structure CreateRectangularStructureCenteredContourPlaneThicknessNotEqualDosePlaneThickness(GridIndexType zPlane);

            /* Generate rectangular structure for the z slices fromm fromZPlane(included) to toZPlane(included)
            */
            core::Structure CreateRectangularStructureCentered(GridIndexType fromZPlane, GridIndexType toZPlane);

			core::Structure CreateTestStructureCircle(GridIndexType zPlane);

			core::Structure CreateRectangularStructureUpperLeftRotated(GridIndexType zPlane);

			core::Structure CreateTestStructureSeveralSeperateSectionsInsideOneVoxel(GridIndexType zPlane);

			core::Structure CreateRectangularStructureCenteredRotatedIntermediatePlacement(
			    GridIndexType zPlane);
			core::Structure CreateRectangularStructureCenteredRotatedIntermediatePlacementLowerRight();
			core::Structure
			CreateRectangularStructureCenteredRotatedIntermediatePlacementLowerRightCounterClock(
			    GridIndexType zPlane);
			core::Structure
			CreateRectangularStructureCenteredRotatedIntermediatePlacementLowerRightCounterClockIntermediatePoints(
			    GridIndexType zPlane);

			core::Structure CreateTestStructureSelfTouchingA(GridIndexType zPlane);

			core::Structure CreateTestStructureIntersecting(GridIndexType zPlane);
			core::Structure CreateTestStructureIntersectingTwoPolygons(GridIndexType zPlane);
			core::Structure CreateTestStructureIntersectingTwoPolygonsInDifferentSlices(GridIndexType zPlane1,
			        GridIndexType zPlane2);

			core::Structure CreateTestStructureInsideInsideTouches(GridIndexType zPlane);
			core::Structure CreateTestStructureInsideInsideTouchesRotatedQuaterPi(GridIndexType zPlane);
			core::Structure CreateTestStructureInsideInsideTouchesCounterClockRotatedQuaterPi(
			    GridIndexType zPlane);
			core::Structure CreateTestStructureInsideInsideTouchesCounterClockRotatedOnePointFivePi(
			    GridIndexType zPlane);
			core::Structure CreateTestStructureInsideInsideTouchesRotatedPointDoubeled(GridIndexType zPlane);

			core::Structure CreateTestStructureInsideInsideTouchesUpperLeft(GridIndexType zPlane);
			core::Structure CreateTestStructureInsideInsideTouchesLowerLeft(GridIndexType zPlane);
			core::Structure CreateTestStructureInsideInsideTouchesLowerRight(GridIndexType zPlane);
			core::Structure CreateTestStructureInsideInsideTouchesUpperRight(GridIndexType zPlane);

			void ShowTestStructure(core::Structure aStructure);
		};
	}//testing
}//rttb

