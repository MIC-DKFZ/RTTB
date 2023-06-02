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
#include "rttbGeometricInfo.h"

namespace rttb
{
	namespace testing
	{

		/*!@brief GeometricInfoTest - test the API of GeometricInfo
			@note ITK pixel indexing: Index[0] = col, Index[1] = row, Index[2] = slice.
			1) test default constructor (values as expected?)
			2) test set/getImagePositionPatient
			4) test set/getSpacing
			5) test set/getNumColumns/Rows/Slices
			6) test get/setOrientationMatrix
			8) test operators "=="
			9) test equalsAlmost
			10) test world to index coordinate conversion
			11) test isInside and index to world coordinate conversion
            12) test with simple Geometry: isInside, continuousIndexToWorldCoordinate(), worldCoordinateToContinuousIndex(), indexToWorldCoordinate()
			13) test getNumberOfVoxels
			14) Test convert, validID and validIndex
			15) Cloning of information
		*/

		int GeometricInfoTest(int /*argc*/, char* /*argv*/[])
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

			//4) test set/getSpacing
			//negative spacing does not make sense!
			/*!@is related to #2028 Should SpacingTypeVector/GridVolumeType/OrientationMatrix be forced to be non-negative?*/
      SpacingVectorType3D expectedSpacing(4.15, 2.35, 100);
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

			//8) test operators "=="
			core::GeometricInfo geoInfo2;
			CHECK_EQUAL(geoInfo, geoInfo);
			CHECK(!(geoInfo == geoInfo2));

			CHECK_EQUAL(geoInfo.getOrientationMatrix(), testOM);
			CHECK(!(geoInfo.getOrientationMatrix() == testNullOM));

			//9) test equalsALmost
			OrientationMatrix testOM2 = testOM;
			SpacingVectorType3D testSPV2 = expectedSpacing;
			WorldCoordinate3D testIPP2 = testIPP;

			core::GeometricInfo testGI2, testGIEmpty;
			testGI2.setImagePositionPatient(testIPP2);
			testGI2.setOrientationMatrix(testOM2);
			testGI2.setSpacing(testSPV2);

			double smallValue = 0.000000001;
			testOM(0, 0) += smallValue;
			testSPV2(2) += smallValue;
			testIPP2(1) += smallValue;

			core::GeometricInfo testGI2similar;
			testGI2similar.setImagePositionPatient(testIPP2);
			testGI2similar.setOrientationMatrix(testOM2);
			testGI2similar.setSpacing(testSPV2);

			CHECK_EQUAL(testGI2.equalsAlmost(testGI2similar), true);
			CHECK_EQUAL(testGI2similar.equalsAlmost(testGI2), true);
			CHECK_EQUAL(testGI2.equalsAlmost(testGI2similar, smallValue * 0.001), false);
			CHECK_EQUAL(testGIEmpty.equalsAlmost(testGI2), false);
			CHECK_EQUAL(testGI2.equalsAlmost(testGIEmpty), false);

			//10) test world to index coordinate conversion
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

			//11) test isInside and index to world coordinate conversion
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

            WorldCoordinate3D testWorldCoordinate;
			ContinuousVoxelGridIndex3D testDoubleIndex;

			ContinuousVoxelGridIndex3D doubleIndex1 = ContinuousVoxelGridIndex3D(0.1, 0, -0.3);
			const WorldCoordinate3D expectedDoubleIndex1(20.1, 100, -1000.3);
			ContinuousVoxelGridIndex3D doubleIndex2 = ContinuousVoxelGridIndex3D(11, 6, 15); //outside
			const WorldCoordinate3D expectedDoubleIndex2(31, 106, -985);
			ContinuousVoxelGridIndex3D doubleIndex3 = ContinuousVoxelGridIndex3D(10.1, 5.0,
			                                      3.0); // outside: Grid dimension = [10,5,3]
			const WorldCoordinate3D expectedDoubleIndex3(30.1, 105, -997);
			ContinuousVoxelGridIndex3D doubleIndex4 = ContinuousVoxelGridIndex3D(0.0, 0.0, 0.0);
			const WorldCoordinate3D expectedDoubleIndex4 = geoInfo.getImagePositionPatient();

			//test double index to world coordinate
			geoInfo.continuousIndexToWorldCoordinate(doubleIndex1, testWorldCoordinate);
			CHECK_EQUAL(testWorldCoordinate, expectedDoubleIndex1);
			geoInfo.continuousIndexToWorldCoordinate(doubleIndex2, testWorldCoordinate);
			CHECK_EQUAL(testWorldCoordinate, expectedDoubleIndex2);
			geoInfo.continuousIndexToWorldCoordinate(doubleIndex3, testWorldCoordinate);
			CHECK_EQUAL(testWorldCoordinate, expectedDoubleIndex3);
			geoInfo.continuousIndexToWorldCoordinate(doubleIndex4, testWorldCoordinate);
			CHECK_EQUAL(testWorldCoordinate, expectedDoubleIndex4);

			geoInfo.worldCoordinateToContinuousIndex(expectedDoubleIndex4, testDoubleIndex);
			CHECK_EQUAL(testDoubleIndex, doubleIndex4);
			geoInfo.worldCoordinateToContinuousIndex(expectedDoubleIndex3, testDoubleIndex);
			CHECK_CLOSE(testDoubleIndex(0), doubleIndex3(0), errorConstant);
			CHECK_CLOSE(testDoubleIndex(1), doubleIndex3(1), errorConstant);
			CHECK_CLOSE(testDoubleIndex(2), doubleIndex3(2), errorConstant);
			geoInfo.worldCoordinateToContinuousIndex(expectedDoubleIndex2, testDoubleIndex);
			CHECK_EQUAL(testDoubleIndex, doubleIndex2);
			geoInfo.worldCoordinateToContinuousIndex(expectedDoubleIndex1, testDoubleIndex);
			CHECK_CLOSE(testDoubleIndex(0), doubleIndex1(0), errorConstant);
			CHECK_CLOSE(testDoubleIndex(1), doubleIndex1(1), errorConstant);
			CHECK_CLOSE(testDoubleIndex(2), doubleIndex1(2), errorConstant);

			VoxelGridIndex3D testIntIndex;
			geoInfo.worldCoordinateToIndex(expectedDoubleIndex4, testIntIndex);
			CHECK_EQUAL(testIntIndex, insideTest1);
			geoInfo.worldCoordinateToIndex(expectedDoubleIndex1, testIntIndex);
			CHECK_EQUAL(testIntIndex, insideTest1);
			geoInfo.worldCoordinateToIndex(expectedDoubleIndex3, testIntIndex);
			CHECK_EQUAL(testIntIndex, expectedVoxelDims);


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
            //no half voxel shift anymore because we changed indexToWorldCoordinate/worldCoordinateToIndex
			insideTest1 = VoxelGridIndex3D(0, 0, 0); //origin (inside)
			const WorldCoordinate3D expectedIndex1(20, 100, -1000);
			insideTest2 = VoxelGridIndex3D(6, 0, 2); //inside
			const WorldCoordinate3D expectedIndex2(23, 94, -1000);
			insideTest3 = VoxelGridIndex3D(11, 6, 15); //outside
			const WorldCoordinate3D expectedIndex3(25.5, 55, -994);
			insideTest4 = VoxelGridIndex3D(10, 5, 3); // outside: Grid dimension = [10,5,3]
			const WorldCoordinate3D expectedIndex4(25, 91, -995);

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

            //12) test with simple Geometry: isInside, continuousIndexToWorldCoordinate(), worldCoordinateToContinuousIndex(), indexToWorldCoordinate()
            core::GeometricInfo geoInfoSimple;
            ImageSize rttbSimpleSize = ImageSize(10, 10, 10);
            geoInfoSimple.setImageSize(rttbSimpleSize);
            SpacingVectorType3D spacingSimple(1, 1, 1);
            geoInfoSimple.setSpacing(spacingSimple);
            OrientationMatrix OMOnes;
            geoInfoSimple.setOrientationMatrix(OMOnes);

            const ContinuousVoxelGridIndex3D doubleIndexPixelOutside1 = ContinuousVoxelGridIndex3D(-0.501, 0.0, 0.0);
            const ContinuousVoxelGridIndex3D doubleIndexPixelOutside2 = ContinuousVoxelGridIndex3D(0.0, 9.501, 0.0);
            const ContinuousVoxelGridIndex3D doubleIndexPixelZero1 = ContinuousVoxelGridIndex3D(0.0, 0.0, 0.0);
            const ContinuousVoxelGridIndex3D doubleIndexPixelZero2 = ContinuousVoxelGridIndex3D(-0.5, -0.5, -0.5);
            const ContinuousVoxelGridIndex3D doubleIndexPixelZero3 = ContinuousVoxelGridIndex3D(0.499999, 0.499999, 0.499999);
            const ContinuousVoxelGridIndex3D doubleIndexPixelOne1 = ContinuousVoxelGridIndex3D(1.0, 0.0, 0.0);
            const ContinuousVoxelGridIndex3D doubleIndexPixelOne2 = ContinuousVoxelGridIndex3D(0.5, 0.499999, 0.499999);
            const ContinuousVoxelGridIndex3D doubleIndexPixelOne3 = ContinuousVoxelGridIndex3D(1.49, -0.5, -0.5);
            const ContinuousVoxelGridIndex3D doubleIndexPixelLast1 = ContinuousVoxelGridIndex3D(9.0, 9.0, 9.0);
            const ContinuousVoxelGridIndex3D doubleIndexPixelLast2 = ContinuousVoxelGridIndex3D(9.4999, 9.4999, 9.4999);
            const ContinuousVoxelGridIndex3D doubleIndexPixelLast3 = ContinuousVoxelGridIndex3D(8.501, 8.501, 8.501);

            const VoxelGridIndex3D indexPixelOutside = VoxelGridIndex3D(11, 0, 0);
            const VoxelGridIndex3D indexPixelZero = VoxelGridIndex3D(0, 0, 0);
            const VoxelGridIndex3D indexPixelOne = VoxelGridIndex3D(1, 0, 0);
            const VoxelGridIndex3D indexPixelLast = VoxelGridIndex3D(9, 9, 9);

            const WorldCoordinate3D worldCoordinateOutside1(doubleIndexPixelOutside1(0), doubleIndexPixelOutside1(1), doubleIndexPixelOutside1(2));
            const WorldCoordinate3D worldCoordinateOutside2(doubleIndexPixelOutside2(0), doubleIndexPixelOutside2(1), doubleIndexPixelOutside2(2));
            const WorldCoordinate3D worldCoordinatePixelZero1(doubleIndexPixelZero1(0), doubleIndexPixelZero1(1), doubleIndexPixelZero1(2));
            const WorldCoordinate3D worldCoordinatePixelZero2(doubleIndexPixelZero2(0), doubleIndexPixelZero2(1), doubleIndexPixelZero2(2));
            const WorldCoordinate3D worldCoordinatePixelZero3(doubleIndexPixelZero3(0), doubleIndexPixelZero3(1), doubleIndexPixelZero3(2));
            const WorldCoordinate3D worldCoordinatePixelOne1(doubleIndexPixelOne1(0), doubleIndexPixelOne1(1), doubleIndexPixelOne1(2));
            const WorldCoordinate3D worldCoordinatePixelOne2(doubleIndexPixelOne2(0), doubleIndexPixelOne2(1), doubleIndexPixelOne2(2));
            const WorldCoordinate3D worldCoordinatePixelOne3(doubleIndexPixelOne3(0), doubleIndexPixelOne3(1), doubleIndexPixelOne3(2));
            const WorldCoordinate3D worldCoordinatePixelLast1(doubleIndexPixelLast1(0), doubleIndexPixelLast1(1), doubleIndexPixelLast1(2));
            const WorldCoordinate3D worldCoordinatePixelLast2(doubleIndexPixelLast2(0), doubleIndexPixelLast2(1), doubleIndexPixelLast2(2));
            const WorldCoordinate3D worldCoordinatePixelLast3(doubleIndexPixelLast3(0), doubleIndexPixelLast3(1), doubleIndexPixelLast3(2));

            bool isInside;
            
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelOutside1, testWorldCoordinate);
            CHECK(!isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelOutside2, testWorldCoordinate);
            CHECK(!isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelZero1, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelZero1);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelZero2, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelZero2);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelZero3, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelZero3);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelOne1, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelOne1);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelOne2, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelOne2);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelOne3, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelOne3);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelLast1, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelLast1);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelLast2, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelLast2);
            CHECK(isInside);
            isInside = geoInfoSimple.continuousIndexToWorldCoordinate(doubleIndexPixelLast3, testWorldCoordinate);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelLast3);
            CHECK(isInside);

            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinateOutside1, testDoubleIndex);
            CHECK(!isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinateOutside2, testDoubleIndex);
            CHECK(!isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelZero1, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelZero1);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelZero2, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelZero2);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelZero3, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelZero3);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelOne1, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelOne1);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelOne2, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelOne2);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelOne3, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelOne3);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelLast1, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelLast1);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelLast2, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelLast2);
            CHECK(isInside);
            isInside = geoInfoSimple.worldCoordinateToContinuousIndex(worldCoordinatePixelLast3, testDoubleIndex);
            CHECK_EQUAL(testDoubleIndex, doubleIndexPixelLast3);
            CHECK(isInside);

            isInside = geoInfoSimple.indexToWorldCoordinate(indexPixelOutside, testWorldCoordinate);
            CHECK(!isInside);
            isInside = geoInfoSimple.indexToWorldCoordinate(indexPixelZero, testWorldCoordinate);
            CHECK(isInside);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelZero1);
            isInside = geoInfoSimple.indexToWorldCoordinate(indexPixelOne, testWorldCoordinate);
            CHECK(isInside);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelOne1);
            isInside = geoInfoSimple.indexToWorldCoordinate(indexPixelLast, testWorldCoordinate);
            CHECK(isInside);
            CHECK_EQUAL(testWorldCoordinate, worldCoordinatePixelLast1);

            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinateOutside1, testIntIndex);
            CHECK(!isInside);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinateOutside2, testIntIndex);
            CHECK(!isInside);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelZero1, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelZero);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelZero2, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelZero);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelZero3, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelZero);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelOne1, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelOne);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelOne2, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelOne);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelOne3, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelOne);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelLast1, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelLast);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelLast2, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelLast);
            isInside = geoInfoSimple.worldCoordinateToIndex(worldCoordinatePixelLast3, testIntIndex);
            CHECK(isInside);
            CHECK_EQUAL(testIntIndex, indexPixelLast);

			//13) test getNumberOfVoxels
			CHECK_EQUAL(expectedVoxelDims(0)*expectedVoxelDims(1)*expectedVoxelDims(2),
			            geoInfo.getNumberOfVoxels());

			//14) Test convert, validID and validIndex
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
			VoxelGridIndex3D aIndex(9999999,999999,999999);
			VoxelGridID aId = 99999999;

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

			//15) Test of GeometricInfo cloning
			core::GeometricInfo sourceInfo;
			sourceInfo.setNumColumns(50);
			sourceInfo.setNumRows(30);
			sourceInfo.setNumSlices(40);
			sourceInfo.setSpacing(expectedSpacing);
			sourceInfo.setOrientationMatrix(testOM);

			core::GeometricInfo clone1 = core::GeometricInfo(sourceInfo);
			CHECK(clone1 == sourceInfo);

			core::GeometricInfo::Pointer clone2 = sourceInfo.clone();
			CHECK(*clone2 == sourceInfo);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
