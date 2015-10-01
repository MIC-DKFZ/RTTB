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

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"

namespace rttb
{
	namespace testing
	{

		/*!@brief GeometricInfoTest - test the API of GeometricInfo
			@note ITK pixel indexing: Index[0] = col, Index[1] = row, Index[2] = slice.
			1) test default constructor (values as expected?)
			2) test set/getImagePositionPatient
			3) test set/getPixelSpacingColumn/Row/SliceThickness
			4) test set/getSpacing
			5) test set/getNumColumns/Rows/Slices
			6) test get/setOrientationMatrix
			7) test getImageOrientationRow/Column
			8) test operators "=="
			9) test world to index coordinate conversion
			10) test isInside and index to world coordinate conversion
			11) test getNumberOfVoxels
		*/

		int GeometricInfoTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//1) test default constructor (values as expected?)
			CHECK_NO_THROW(core::GeometricInfo());
			core::GeometricInfo geoInfo;
			SpacingVectorType3D testNullSV(0);
			WorldCoordinate3D testNullWC(0);
			OrientationMatrix testNullOM(0);
			CHECK_EQUAL(testNullSV, geoInfo.getSpacing());
			CHECK_EQUAL(testNullWC, geoInfo.getImagePositionPatient());
			CHECK_EQUAL(testNullOM, geoInfo.getOrientationMatrix());

			//2) test set/getImagePositionPatient
			WorldCoordinate3D testIPP(1.2, 3.4, 5.6);
			CHECK_NO_THROW(geoInfo.setImagePositionPatient(testIPP));
			geoInfo.setImagePositionPatient(testIPP);
			CHECK_EQUAL(testIPP, geoInfo.getImagePositionPatient());

			//3) test set/getPixelSpacingColumn/Row/SliceThickness
			SpacingVectorType3D expectedSpacing(1, 2.2, 3.3);
			CHECK_NO_THROW(geoInfo.setPixelSpacingRow(expectedSpacing(0)));
			CHECK_EQUAL(expectedSpacing(0), geoInfo.getPixelSpacingRow());
			CHECK_EQUAL(SpacingVectorType3D(expectedSpacing(0), 0, 0), geoInfo.getSpacing());
			CHECK_EQUAL(geoInfo.getPixelSpacingRow(), geoInfo.getSpacing()(0));

			CHECK_NO_THROW(geoInfo.setPixelSpacingColumn(expectedSpacing(1)));
			CHECK_EQUAL(expectedSpacing(1), geoInfo.getPixelSpacingColumn());
			CHECK_EQUAL(SpacingVectorType3D(expectedSpacing(0), expectedSpacing(1), 0), geoInfo.getSpacing());
			CHECK_EQUAL(geoInfo.getPixelSpacingColumn(), geoInfo.getSpacing()(1));

			CHECK_NO_THROW(geoInfo.setSliceThickness(expectedSpacing(2)));
			CHECK_EQUAL(expectedSpacing(2), geoInfo.getSliceThickness());
			CHECK_EQUAL(expectedSpacing, geoInfo.getSpacing());
			CHECK_EQUAL(geoInfo.getSliceThickness(), geoInfo.getSpacing()(2));

			//4) test set/getSpacing
			//negative spacing does not make sense!
			/*!@is related to #2028 Should SpacingTypeVector/GridVolumeType/OrientationMatrix be forced to be non-negative?*/
			expectedSpacing(0) = 4.15;
			expectedSpacing(1) = 2.35;
			expectedSpacing(2) = 100;
			CHECK_NO_THROW(geoInfo.setSpacing(expectedSpacing));
			geoInfo.setSpacing(expectedSpacing);
			CHECK_EQUAL(expectedSpacing, geoInfo.getSpacing());

			//5) test set/getNumColumns/Rows/Slices
			const VoxelGridIndex3D expectedVoxelDims(10, 5, 3);
			//CHECK_THROW(geoInfo.setNumColumns(1.2)); -> implicit conversion will prevent exception
			CHECK_NO_THROW(geoInfo.setNumColumns(expectedVoxelDims(0)));
			geoInfo.setNumColumns(expectedVoxelDims(0));

			CHECK_NO_THROW(geoInfo.setNumRows(expectedVoxelDims(1)));
			geoInfo.setNumRows(expectedVoxelDims(1));

			//CHECK_THROW(geoInfo.setNumSlices(4.2));  -> implicit conversion will prevent exception
			CHECK_NO_THROW(geoInfo.setNumSlices(expectedVoxelDims(2)));
			geoInfo.setNumSlices(expectedVoxelDims(2));

			ImageSize rttbSize = geoInfo.getImageSize();
			CHECK_EQUAL(rttbSize(0), geoInfo.getNumColumns());
			CHECK_EQUAL(rttbSize(1), geoInfo.getNumRows());
			CHECK_EQUAL(rttbSize(2), geoInfo.getNumSlices());

			rttbSize = ImageSize(11, 99, 6);

			core::GeometricInfo geoInfo3;
			geoInfo3.setImageSize(rttbSize);
			CHECK_EQUAL(rttbSize(0), geoInfo3.getNumColumns());
			CHECK_EQUAL(rttbSize(1), geoInfo3.getNumRows());
			CHECK_EQUAL(rttbSize(2), geoInfo3.getNumSlices());


			//6) test get/setOrientationMatrix
			CHECK_EQUAL(testNullOM, geoInfo.getOrientationMatrix());
			OrientationMatrix testOM(0);
			const WorldCoordinate3D testIORow(5.5, 4.7, 3.2);
			const WorldCoordinate3D testIOColumn(2.5, 1.8, 9.1);
			WorldCoordinate3D ortho = testIORow.cross(testIOColumn);

			testOM(0, 0) = testIORow(0);
			testOM(1, 0) = testIORow(1);
			testOM(2, 0) = testIORow(2);
			CHECK_NO_THROW(geoInfo.setOrientationMatrix(testOM));
			geoInfo.setOrientationMatrix(testOM);
			CHECK_EQUAL(testOM, geoInfo.getOrientationMatrix());

			testOM(0, 1) = testIOColumn(0);
			testOM(1, 1) = testIOColumn(1);
			testOM(2, 1) = testIOColumn(2);
			CHECK_NO_THROW(geoInfo.setOrientationMatrix(testOM));
			geoInfo.setOrientationMatrix(testOM);
			CHECK_EQUAL(testOM, geoInfo.getOrientationMatrix());

			testOM(0, 2) = ortho(0);
			testOM(1, 2) = ortho(1);
			testOM(2, 2) = ortho(2);
			CHECK_NO_THROW(geoInfo.setOrientationMatrix(testOM));
			geoInfo.setOrientationMatrix(testOM);
			CHECK_EQUAL(testOM, geoInfo.getOrientationMatrix());

			//7) test getImageOrientationRow/Column
			CHECK_EQUAL(testIORow, geoInfo.getImageOrientationRow());
			CHECK_EQUAL(testIOColumn, geoInfo.getImageOrientationColumn());

			//8) test operators "=="
			core::GeometricInfo geoInfo2;
			CHECK_EQUAL(geoInfo, geoInfo);
			CHECK(!(geoInfo == geoInfo2));

			CHECK_EQUAL(geoInfo.getOrientationMatrix(), testOM);
			CHECK(!(geoInfo.getOrientationMatrix() == testNullOM));

			//9) test world to index coordinate converion
			//use unit matrix as orientation matrix
			CHECK_NO_THROW(geoInfo.setOrientationMatrix(OrientationMatrix()));

			//origin (inside)
			WorldCoordinate3D insideTestWC1 = geoInfo.getImagePositionPatient();
			//inside
			const VoxelGridIndex3D expectedIndex(8, 3, 2);
			WorldCoordinate3D insideTestWC2(expectedIndex(0)*expectedSpacing(0) + testIPP(0),
			                                expectedIndex(1)*expectedSpacing(1) + testIPP(1), expectedIndex(2)*expectedSpacing(2) + testIPP(2));
			//outside
			WorldCoordinate3D insideTestWC3(-33.12, 0, 14);
			// outside (dimension of grid)
			WorldCoordinate3D insideTestWC4(expectedVoxelDims(0)*expectedSpacing(0) + testIPP(0),
			                                expectedVoxelDims(1)*expectedSpacing(1) + testIPP(1),
			                                expectedVoxelDims(2)*expectedSpacing(2) + testIPP(2));
			CHECK(geoInfo.isInside(insideTestWC1));
			CHECK(geoInfo.isInside(insideTestWC2));
			CHECK(!(geoInfo.isInside(insideTestWC3)));
			CHECK(!(geoInfo.isInside(insideTestWC4)));

			VoxelGridIndex3D testConvert(0);
			CHECK(geoInfo.worldCoordinateToIndex(insideTestWC1, testConvert));
			CHECK(geoInfo.isInside(testConvert));
			CHECK_EQUAL(VoxelGridIndex3D(0), testConvert);
			CHECK(geoInfo.worldCoordinateToIndex(insideTestWC2, testConvert));
			CHECK(geoInfo.isInside(testConvert));
			CHECK_EQUAL(expectedIndex, testConvert);
			CHECK(!(geoInfo.worldCoordinateToIndex(insideTestWC3, testConvert)));
			//CHECK_EQUAL(VoxelGridIndex3D(0),testConvert); //if value is in a negative grid position it will be converted
			//to a very large unrelated number.
			CHECK(!(geoInfo.isInside(testConvert)));
			CHECK(!(geoInfo.worldCoordinateToIndex(insideTestWC4, testConvert)));
			CHECK_EQUAL(expectedVoxelDims, testConvert);
			CHECK(!(geoInfo.isInside(testConvert)));

			//use a more complicated orientation matrix
			OrientationMatrix newOrientation(0);
			newOrientation(0, 0) = 0.5;
			newOrientation(1, 2) = -3;
			newOrientation(2, 1) = 1;
			CHECK_NO_THROW(geoInfo.setOrientationMatrix(newOrientation));

			testIPP = WorldCoordinate3D(20, 100, -1000);
			CHECK_NO_THROW(geoInfo.setImagePositionPatient(testIPP));

			CHECK_NO_THROW(geoInfo.setSpacing(SpacingVectorType3D(1)));

			//values for testing were generated with a dedicated MeVisLab routine
			insideTestWC1 = geoInfo.getImagePositionPatient(); //origin (inside)
			const VoxelGridIndex3D expectedIndexWC1(0, 0, 0);
			insideTestWC2 = WorldCoordinate3D(22.5, 97, -998); //inside
			const VoxelGridIndex3D expectedIndexWC2(5, 2, 1);
			insideTestWC3 = WorldCoordinate3D(26, 88, -996); //outside
			const VoxelGridIndex3D expectedIndexWC3(12, 4, 4);
			insideTestWC4 = WorldCoordinate3D(25, 91, -995); // outside: Grid dimension = [10,5,3]
			const VoxelGridIndex3D expectedIndexWC4(10, 5, 3);

			CHECK(geoInfo.isInside(insideTestWC1));
			CHECK_EQUAL(geoInfo.isInside(insideTestWC1), geoInfo.isInside(expectedIndexWC1));
			CHECK(geoInfo.isInside(insideTestWC2));
			CHECK_EQUAL(geoInfo.isInside(insideTestWC2), geoInfo.isInside(expectedIndexWC2));
			CHECK(!(geoInfo.isInside(insideTestWC3)));
			CHECK_EQUAL(geoInfo.isInside(insideTestWC3), geoInfo.isInside(expectedIndexWC3));
			CHECK(!(geoInfo.isInside(insideTestWC4)));
			CHECK_EQUAL(geoInfo.isInside(insideTestWC4), geoInfo.isInside(expectedIndexWC4));

			CHECK(geoInfo.worldCoordinateToIndex(insideTestWC1, testConvert));
			CHECK(geoInfo.isInside(testConvert));
			CHECK_EQUAL(expectedIndexWC1, testConvert);

			CHECK(geoInfo.worldCoordinateToIndex(insideTestWC2, testConvert));
			CHECK(geoInfo.isInside(testConvert));
			CHECK_EQUAL(expectedIndexWC2, testConvert);

			CHECK(!(geoInfo.worldCoordinateToIndex(insideTestWC3, testConvert)));
			CHECK(!(geoInfo.isInside(testConvert)));
			CHECK_EQUAL(expectedIndexWC3, testConvert);

			CHECK(!(geoInfo.worldCoordinateToIndex(insideTestWC4, testConvert)));
			CHECK(!(geoInfo.isInside(testConvert)));
			CHECK_EQUAL(expectedIndexWC4, testConvert);

			//10) test isInside and index to world coordinate converion
			//use unit matrix as orientation matrix
			CHECK_NO_THROW(geoInfo.setOrientationMatrix(OrientationMatrix()));

			VoxelGridIndex3D insideTest1(0, 0, 0); //origin (inside)
			VoxelGridIndex3D insideTest2(2, 3, 1); //inside
			VoxelGridIndex3D insideTest3(0, 6, 14); //outside
			VoxelGridIndex3D insideTest4 = expectedVoxelDims; // outside
			CHECK(geoInfo.isInside(insideTest1));
			CHECK(geoInfo.isInside(insideTest2));
			CHECK(!(geoInfo.isInside(insideTest3)));
			CHECK(!(geoInfo.isInside(insideTest4)));

			WorldCoordinate3D testInside(0);
			CHECK(geoInfo.indexToWorldCoordinate(insideTest1, testInside));
			CHECK(geoInfo.isInside(testInside));
			//CHECK_EQUAL(geoInfo.getImagePositionPatient(),testInside); //half voxel shift prevents equality!
			CHECK(geoInfo.indexToWorldCoordinate(insideTest2, testInside));
			CHECK(geoInfo.isInside(testInside));
			CHECK(!(geoInfo.indexToWorldCoordinate(insideTest3, testInside)));
			CHECK(!(geoInfo.isInside(testInside)));
			CHECK(!(geoInfo.indexToWorldCoordinate(insideTest4, testInside)));
			CHECK(!(geoInfo.isInside(testInside)));

			//use a more complicated orientation matrix
			newOrientation = OrientationMatrix(0);
			newOrientation(0, 0) = 0.5;
			newOrientation(1, 2) = -3;
			newOrientation(2, 1) = 1;
			CHECK_NO_THROW(geoInfo.setOrientationMatrix(newOrientation));

			testIPP = WorldCoordinate3D(20, 100, -1000);
			CHECK_NO_THROW(geoInfo.setImagePositionPatient(testIPP));

			CHECK_NO_THROW(geoInfo.setSpacing(SpacingVectorType3D(1)));

			//values for testing were generated with a dedicated MeVisLab routine
			//expected world coordinates need to accommodate the half voxel shift
			//caused by the use of the central position in the voxel for conversion.
			insideTest1 = VoxelGridIndex3D(0, 0, 0); //origin (inside)
			const WorldCoordinate3D expectedIndex1(19.75, 101.5, -1000.5);
			insideTest2 = VoxelGridIndex3D(6, 0, 2); //inside
			const WorldCoordinate3D expectedIndex2(22.75, 95.5, -1000.5);
			insideTest3 = VoxelGridIndex3D(11, 6, 15); //outside
			const WorldCoordinate3D expectedIndex3(25.25, 56.5, -994.5);
			insideTest4 = VoxelGridIndex3D(10, 5, 3); // outside: Grid dimension = [10,5,3]
			const WorldCoordinate3D expectedIndex4(24.75, 92.5, -995.5);

			CHECK(geoInfo.isInside(insideTest1));
			CHECK_EQUAL(geoInfo.isInside(insideTest1), geoInfo.isInside(expectedIndex1));
			CHECK(geoInfo.isInside(insideTest2));
			CHECK_EQUAL(geoInfo.isInside(insideTest2), geoInfo.isInside(expectedIndex2));
			CHECK(!(geoInfo.isInside(insideTest3)));
			CHECK_EQUAL(geoInfo.isInside(insideTest3), geoInfo.isInside(expectedIndex3));
			CHECK(!(geoInfo.isInside(insideTest4)));
			CHECK_EQUAL(geoInfo.isInside(insideTest4), geoInfo.isInside(expectedIndex4));

			CHECK(geoInfo.indexToWorldCoordinate(insideTest1, testInside));
			CHECK(geoInfo.isInside(testInside));
			CHECK_EQUAL(expectedIndex1, testInside);

			CHECK(geoInfo.indexToWorldCoordinate(insideTest2, testInside));
			CHECK(geoInfo.isInside(testInside));
			CHECK_EQUAL(expectedIndex2, testInside);

			CHECK(!(geoInfo.indexToWorldCoordinate(insideTest3, testInside)));
			CHECK(!(geoInfo.isInside(testInside)));
			CHECK_EQUAL(expectedIndex3, testInside);

			CHECK(!(geoInfo.indexToWorldCoordinate(insideTest4, testInside)));
			CHECK(!(geoInfo.isInside(testInside)));
			CHECK_EQUAL(expectedIndex4, testInside);

			//11) test getNumberOfVoxels
			CHECK_EQUAL(expectedVoxelDims(0)*expectedVoxelDims(1)*expectedVoxelDims(2),
			            geoInfo.getNumberOfVoxels());

			//12) Test convert, validID and validIndex
			geoInfo.setNumColumns(50);
			geoInfo.setNumRows(30);
			geoInfo.setNumSlices(40);

			VoxelGridIndex3D startIndex(0, 0, 0);
			VoxelGridID startId(0);
			VoxelGridIndex3D endIndex(geoInfo.getNumColumns() - 1, geoInfo.getNumRows() - 1,
			                          geoInfo.getNumSlices() - 1);
			VoxelGridID endId((geoInfo.getNumColumns()*geoInfo.getNumRows()*geoInfo.getNumSlices()) - 1);
			VoxelGridIndex3D indexInvalid(geoInfo.getNumColumns(), geoInfo.getNumRows(),
			                              geoInfo.getNumSlices());
			VoxelGridID idInvalid(geoInfo.getNumColumns()*geoInfo.getNumRows()*geoInfo.getNumSlices());

			CHECK(geoInfo.validID(startId));
			CHECK(geoInfo.validIndex(startIndex));
			VoxelGridIndex3D aIndex;
			VoxelGridID aId;

			CHECK(geoInfo.convert(startIndex, aId));
			CHECK(geoInfo.convert(startId, aIndex));
			CHECK_EQUAL(aId, startId);
			CHECK_EQUAL(aIndex, startIndex);

			CHECK(geoInfo.validID(endId));
			CHECK(geoInfo.validIndex(endIndex));

			CHECK(geoInfo.convert(endIndex, aId));
			CHECK(geoInfo.convert(endId, aIndex));
			CHECK_EQUAL(aId, endId);
			CHECK_EQUAL(aIndex, endIndex);

			CHECK(!geoInfo.validID(idInvalid));
			CHECK(!geoInfo.validIndex(indexInvalid));
			CHECK(!geoInfo.convert(idInvalid, aIndex));
			CHECK(!geoInfo.convert(indexInvalid, aId));

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
