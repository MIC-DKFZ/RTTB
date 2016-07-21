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

#include <math.h>

#include "DummyStructure.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbInvalidDoseException.h"

namespace rttb
{
	namespace testing
	{

		DummyStructure::~DummyStructure() {}

		DummyStructure::DummyStructure(const core::GeometricInfo& aGeoInfo)
		{
			_geoInfo = aGeoInfo;
		}

		core::Structure DummyStructure::CreateRectangularStructureCentered(GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;
			VoxelGridIndex2D another_i1(2, 1);
			VoxelGridIndex2D another_i2(5, 1);
			VoxelGridIndex2D another_i3(5, 5);
			VoxelGridIndex2D another_i4(2, 5);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonCenter(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure_rectangular_centered = core::Structure(another_polySeq);

			return test_structure_rectangular_centered;
		}

        core::Structure DummyStructure::CreateRectangularStructureCentered(GridIndexType fromZPlane, GridIndexType toZPlane)
        {
            CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

            std::vector<VoxelGridIndex2D> another_voxelVector;
            VoxelGridIndex2D another_i1(2, 1);
            VoxelGridIndex2D another_i2(5, 1);
            VoxelGridIndex2D another_i3(5, 5);
            VoxelGridIndex2D another_i4(2, 5);
            PolygonSequenceType another_polySeq;

            for (int i = fromZPlane; i <= toZPlane; ++i){
                another_voxelVector.clear();
                another_voxelVector.push_back(another_i1);
                another_voxelVector.push_back(another_i2);
                another_voxelVector.push_back(another_i3);
                another_voxelVector.push_back(another_i4);
                PolygonType another_polygon1 = another_cts.createPolygonCenter(another_voxelVector, i);

                
                another_polySeq.push_back(another_polygon1);
            }

            core::Structure test_structure_rectangular_centered = core::Structure(another_polySeq);

            return test_structure_rectangular_centered;
        }

		core::Structure DummyStructure::CreateRectangularStructureCenteredRotatedIntermediatePlacement(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;
			VoxelGridIndex2D another_i1(5, 1);
			VoxelGridIndex2D another_i2(8, 4);
			VoxelGridIndex2D another_i3(5, 7);
			VoxelGridIndex2D another_i4(2, 4);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonBetweenUpperLeftAndCenter(
			                                   another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure
		DummyStructure::CreateRectangularStructureCenteredRotatedIntermediatePlacementLowerRight()
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;
			VoxelGridIndex2D another_i1(5, 1);
			VoxelGridIndex2D another_i2(8, 4);
			VoxelGridIndex2D another_i3(5, 7);
			VoxelGridIndex2D another_i4(2, 4);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);

			PolygonSequenceType another_polySeq;

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure
		DummyStructure::CreateRectangularStructureCenteredRotatedIntermediatePlacementLowerRightCounterClock(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 4);
			VoxelGridIndex2D another_i2(5, 7);
			VoxelGridIndex2D another_i3(8, 4);
			VoxelGridIndex2D another_i4(5, 1);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonBetweenLowerRightAndCenter(
			                                   another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure
		DummyStructure::CreateRectangularStructureCenteredRotatedIntermediatePlacementLowerRightCounterClockIntermediatePoints(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 4);
			VoxelGridIndex2D another_i2(5, 7);
			VoxelGridIndex2D another_i3(8, 4);
			VoxelGridIndex2D another_i4(5, 1);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonIntermediatePoints(another_voxelVector ,
			                               zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureSeveralSeperateSectionsInsideOneVoxel(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 2);

			another_voxelVector.push_back(another_i1);

			PolygonType another_polygon1 = another_cts.createStructureSeveralSectionsInsideOneVoxelA(
			                                   another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureSelfTouchingA(GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 2);

			another_voxelVector.push_back(another_i1);

			PolygonType another_polygon1 = another_cts.createStructureSelfTouchingA(another_voxelVector ,
			                               zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureIntersectingTwoPolygonsInDifferentSlices(
		    GridIndexType zPlane1,
		    GridIndexType zPlane2)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);
			CreateTestStructure one_more_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 4);
			VoxelGridIndex2D another_i2(8, 4);
			VoxelGridIndex2D another_i3(8, 6);
			VoxelGridIndex2D another_i4(2, 6);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonCenter(another_voxelVector , zPlane1);

			std::vector<VoxelGridIndex2D> one_more_voxelVector;

			VoxelGridIndex2D one_more_i1(3, 5);
			VoxelGridIndex2D one_more_i2(9, 5);
			VoxelGridIndex2D one_more_i3(9, 7);
			VoxelGridIndex2D one_more_i4(3, 7);

			one_more_voxelVector.push_back(one_more_i1);
			one_more_voxelVector.push_back(one_more_i2);
			one_more_voxelVector.push_back(one_more_i3);
			one_more_voxelVector.push_back(one_more_i4);
			PolygonType another_polygon2 = one_more_cts.createPolygonCenter(one_more_voxelVector , zPlane2);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);
			another_polySeq.push_back(another_polygon2);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureIntersectingTwoPolygons(GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);
			CreateTestStructure one_more_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 4);
			VoxelGridIndex2D another_i2(8, 4);
			VoxelGridIndex2D another_i3(8, 6);
			VoxelGridIndex2D another_i4(2, 6);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonCenter(another_voxelVector , zPlane);

			std::vector<VoxelGridIndex2D> one_more_voxelVector;

			VoxelGridIndex2D one_more_i1(3, 5);
			VoxelGridIndex2D one_more_i2(9, 5);
			VoxelGridIndex2D one_more_i3(9, 7);
			VoxelGridIndex2D one_more_i4(3, 7);

			one_more_voxelVector.push_back(one_more_i1);
			one_more_voxelVector.push_back(one_more_i2);
			one_more_voxelVector.push_back(one_more_i3);
			one_more_voxelVector.push_back(one_more_i4);
			PolygonType another_polygon2 = one_more_cts.createPolygonCenter(one_more_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);
			another_polySeq.push_back(another_polygon2);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure  DummyStructure::CreateTestStructureIntersecting(GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 4);
			VoxelGridIndex2D another_i2(8, 4);
			VoxelGridIndex2D another_i3(2, 6);
			VoxelGridIndex2D another_i4(8, 6);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonCenter(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouches(GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(3, 4);
			VoxelGridIndex2D another_i2(2, 8);
			VoxelGridIndex2D another_i3(4, 8);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);

			PolygonType another_polygon1 = another_cts.createPolygonUpperCenter(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouchesRotatedPointDoubeled(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 4);
			VoxelGridIndex2D another_i4(2, 4);
			VoxelGridIndex2D another_i2(4, 4);
			VoxelGridIndex2D another_i3(3, 8);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i4);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);

			PolygonType another_polygon1 = another_cts.createPolygonUpperCenter(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure
		DummyStructure::CreateTestStructureInsideInsideTouchesCounterClockRotatedOnePointFivePi(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(3, 5);
			VoxelGridIndex2D another_i2(3, 5);
			VoxelGridIndex2D another_i3(7, 6);
			VoxelGridIndex2D another_i4(7, 6);
			VoxelGridIndex2D another_i5(7, 4);
			VoxelGridIndex2D another_i6(7, 4);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			another_voxelVector.push_back(another_i5);
			another_voxelVector.push_back(another_i6);

			PolygonType another_polygon1 = another_cts.createPolygonLeftEdgeMiddle(another_voxelVector ,
			                               zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouchesCounterClockRotatedQuaterPi(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(7, 5);
			VoxelGridIndex2D another_i2(7, 5);
			VoxelGridIndex2D another_i3(3, 6);
			VoxelGridIndex2D another_i4(3, 6);
			VoxelGridIndex2D another_i5(3, 4);
			VoxelGridIndex2D another_i6(3, 4);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			another_voxelVector.push_back(another_i5);
			another_voxelVector.push_back(another_i6);

			PolygonType another_polygon1 = another_cts.createPolygonLeftEdgeMiddle(another_voxelVector ,
			                               zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouchesRotatedQuaterPi(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(3, 4);
			VoxelGridIndex2D another_i2(3, 4);
			VoxelGridIndex2D another_i3(3, 6);
			VoxelGridIndex2D another_i4(3, 6);
			VoxelGridIndex2D another_i5(7, 5);
			VoxelGridIndex2D another_i6(7, 5);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			another_voxelVector.push_back(another_i5);
			another_voxelVector.push_back(another_i6);

			PolygonType another_polygon1 = another_cts.createPolygonLeftEdgeMiddle(another_voxelVector ,
			                               zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureCircle(GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(4, 4);

			another_voxelVector.push_back(another_i1);

			PolygonType another_polygon1 = another_cts.createPolygonCircle(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouchesUpperRight(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;


			VoxelGridIndex2D another_i1(3, 4);
			VoxelGridIndex2D another_i3(4, 5);
			VoxelGridIndex2D another_i5(5, 3);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i5);

			PolygonType another_polygon1 = another_cts.createPolygonLeftUpper(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouchesLowerRight(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(2, 2);
			VoxelGridIndex2D another_i2(2, 2);
			VoxelGridIndex2D another_i3(3, 1);
			VoxelGridIndex2D another_i4(3, 1);
			VoxelGridIndex2D another_i5(4, 3);
			VoxelGridIndex2D another_i6(4, 3);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			another_voxelVector.push_back(another_i5);
			another_voxelVector.push_back(another_i6);

			PolygonType another_polygon1 = another_cts.createPolygonLeftUpper(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouchesLowerLeft(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(3, 3);
			VoxelGridIndex2D another_i2(3, 3);
			VoxelGridIndex2D another_i3(4, 4);
			VoxelGridIndex2D another_i4(4, 4);
			VoxelGridIndex2D another_i5(2, 5);
			VoxelGridIndex2D another_i6(2, 5);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			another_voxelVector.push_back(another_i5);
			another_voxelVector.push_back(another_i6);

			PolygonType another_polygon1 = another_cts.createPolygonLeftUpper(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		core::Structure DummyStructure::CreateTestStructureInsideInsideTouchesUpperLeft(
		    GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;

			VoxelGridIndex2D another_i1(5, 5);
			VoxelGridIndex2D another_i2(5, 5);
			VoxelGridIndex2D another_i3(4, 6);
			VoxelGridIndex2D another_i4(4, 6);
			VoxelGridIndex2D another_i5(3, 4);
			VoxelGridIndex2D another_i6(3, 4);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			another_voxelVector.push_back(another_i5);
			another_voxelVector.push_back(another_i6);

			PolygonType another_polygon1 = another_cts.createPolygonLeftUpper(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

		void DummyStructure::ShowTestStructure(core::Structure aStructure)
		{
			WorldCoordinate3D aPoint(0);

			PolygonSequenceType strVector = aStructure.getStructureVector();

			for (size_t struct_index = 0 ; struct_index < strVector.size() ; struct_index++)
			{

				for (size_t point_index = 0 ; point_index < strVector.at(struct_index).size() ; point_index++)
				{

					aPoint = strVector.at(struct_index).at(point_index);

					std::cout << " aPoint.x "  << aPoint.x()  << std::endl;
					std::cout << " aPoint.y "  << aPoint.y()  << std::endl;
					std::cout << " aPoint.z "  << aPoint.z()  << std::endl;
				}

			}
		}

		core::Structure DummyStructure::CreateRectangularStructureUpperLeftRotated(GridIndexType zPlane)
		{
			CreateTestStructure another_cts = CreateTestStructure(_geoInfo);

			std::vector<VoxelGridIndex2D> another_voxelVector;
			VoxelGridIndex2D another_i1(5, 1);
			VoxelGridIndex2D another_i2(8, 4);
			VoxelGridIndex2D another_i3(5, 7);
			VoxelGridIndex2D another_i4(2, 4);

			another_voxelVector.push_back(another_i1);
			another_voxelVector.push_back(another_i2);
			another_voxelVector.push_back(another_i3);
			another_voxelVector.push_back(another_i4);
			PolygonType another_polygon1 = another_cts.createPolygonLeftUpper(another_voxelVector , zPlane);

			PolygonSequenceType another_polySeq;
			another_polySeq.push_back(another_polygon1);

			core::Structure test_structure = core::Structure(another_polySeq);

			return test_structure;
		}

	}//testing
}//rttb

