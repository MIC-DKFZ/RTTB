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

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbOTBMaskAccessor.h"
#include "rttbMaskVoxel.h"
#include "rttbNullPointerException.h"
#include "rttbException.h"
#include "../core/DummyStructure.h"
#include "../core/DummyDoseAccessor.h"
#include "rttbMaskVoxelListTester.h"
#include "rttbMaskRectStructTester.h"


namespace rttb
{
	namespace testing
	{

		/*! @brief GenericMaskedDoseIteratorTest.
			1) test constructors
			2) test updateMask (do relevant voxels match?)
			3) test valid/convert
			4) test getMaskAt
			5) other interface functions (simple getters)
		*/
		int OTBMaskAccessorTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef core::Structure::StructTypePointer StructTypePointer;
			typedef masks::OTBMaskAccessor::MaskVoxelListPointer MaskVoxelListPointer;
			typedef masks::OTBMaskAccessor::MaskVoxelList MaskVoxelList;

			// generate test structure set
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor =
			    boost::make_shared<DummyDoseAccessor>();

			DummyStructure myStructGenerator(spTestDoseAccessor->getGeometricInfo());

			GridIndexType zPlane = 4;
			core::Structure myTestStruct = myStructGenerator.CreateRectangularStructureCentered(zPlane);
			StructTypePointer spMyStruct = boost::make_shared<core::Structure>(myTestStruct);


			//1) test constructors
			CHECK_NO_THROW(masks::OTBMaskAccessor(spMyStruct, spTestDoseAccessor->getGeometricInfo()));
			masks::OTBMaskAccessor testOTB1(spMyStruct, spTestDoseAccessor->getGeometricInfo());
			CHECK_EQUAL(spTestDoseAccessor->getGeometricInfo(), testOTB1.getGeometricInfo());

			CHECK_NO_THROW(masks::OTBMaskAccessor(spMyStruct, spTestDoseAccessor->getGeometricInfo()));
			masks::OTBMaskAccessor testOTB2(spMyStruct, spTestDoseAccessor->getGeometricInfo());
			CHECK_EQUAL(spTestDoseAccessor->getGeometricInfo(), testOTB2.getGeometricInfo());

			//2) test updateMask (do relevant voxels match?)
			CHECK_NO_THROW(testOTB1.updateMask());
			CHECK_NO_THROW(testOTB1.getRelevantVoxelVector());
			MaskVoxelListPointer relVoxelOTB1 = testOTB1.getRelevantVoxelVector();

			boost::shared_ptr<masks::OTBMaskAccessor> spMaskAccessor =
			    boost::make_shared<masks::OTBMaskAccessor>(spMyStruct, spTestDoseAccessor->getGeometricInfo());
			MaskRectStructTester voxelizationTester(spMaskAccessor, zPlane);
			CHECK_TESTER(voxelizationTester);

			MaskVoxelListPointer relVoxelOTB2 = testOTB2.getRelevantVoxelVector();

			MaskVoxelListTester listComp(relVoxelOTB1, relVoxelOTB2);
			CHECK_TESTER(listComp);

			//3) test valid/convert
			const VoxelGridIndex3D gridIndexIn1(0, 0, 0);
			const VoxelGridIndex3D gridIndexIn2(4, 3, 4);
			const VoxelGridIndex3D gridIndexIn3(testOTB2.getGeometricInfo().getNumColumns() - 1,
			                                    testOTB2.getGeometricInfo().getNumRows() - 1,
			                                    testOTB2.getGeometricInfo().getNumSlices() - 1);
			const VoxelGridIndex3D gridIndexOut1(testOTB2.getGeometricInfo().getNumColumns(),
			                                     testOTB2.getGeometricInfo().getNumRows(),
			                                     testOTB2.getGeometricInfo().getNumSlices());
			const VoxelGridIndex3D gridIndexOut2(testOTB2.getGeometricInfo().getNumRows() * 2,
			                                     testOTB2.getGeometricInfo().getNumRows() + 5,
			                                     testOTB2.getGeometricInfo().getNumSlices() - 2);

			CHECK(testOTB2.getGeometricInfo().validIndex(gridIndexIn1));
			CHECK(testOTB2.getGeometricInfo().validIndex(gridIndexIn2));
			std::cout << gridIndexIn3 << std::endl;
			CHECK(testOTB2.getGeometricInfo().validIndex(gridIndexIn3));
			CHECK(!testOTB2.getGeometricInfo().validIndex(gridIndexOut1));
			CHECK(!testOTB2.getGeometricInfo().validIndex(gridIndexOut2));

			const VoxelGridID gridIDIn1(0);
			int in2 = gridIndexIn2.z() * testOTB2.getGeometricInfo().getNumColumns() *
			          testOTB2.getGeometricInfo().getNumRows() +
			          gridIndexIn2.y() * testOTB2.getGeometricInfo().getNumColumns() + gridIndexIn2.x();
			const VoxelGridID gridIDIn2(in2);
			const VoxelGridID gridIDIn3(testOTB2.getGeometricInfo().getNumberOfVoxels() - 1); //size-1
			const VoxelGridID gridIDOut1(testOTB2.getGeometricInfo().getNumberOfVoxels()); //size
			const VoxelGridID gridIDOut2(testOTB2.getGeometricInfo().getNumberOfVoxels() + 10);

			CHECK(testOTB2.getGeometricInfo().validID(gridIDIn1));
			CHECK(testOTB2.getGeometricInfo().validID(gridIDIn2));
			CHECK(testOTB2.getGeometricInfo().validID(gridIDIn3));
			CHECK(!testOTB2.getGeometricInfo().validID(gridIDOut1));
			CHECK(!testOTB2.getGeometricInfo().validID(gridIDOut2));

			VoxelGridIndex3D test;
			CHECK(testOTB2.getGeometricInfo().convert(gridIDIn1, test));
			CHECK_EQUAL(gridIndexIn1, test);
			CHECK(testOTB2.getGeometricInfo().convert(gridIDIn2, test));
			CHECK_EQUAL(gridIndexIn2, test);
			CHECK(!testOTB2.getGeometricInfo().convert(gridIDOut1, test));
			//gridIndexOut1 is invalid, test is therefore not asigned, therefore testing is not necessary
			//CHECK_EQUAL(gridIndexOut1,test);
			VoxelGridID testId;
			CHECK(testOTB2.getGeometricInfo().convert(gridIndexIn1, testId));
			CHECK_EQUAL(gridIDIn1, testId);
			CHECK(testOTB2.getGeometricInfo().convert(gridIndexIn2, testId));
			CHECK_EQUAL(gridIDIn2, testId);
			CHECK(!testOTB2.getGeometricInfo().convert(gridIndexOut1, testId));
			//gridIDOut1 is invalid, testId is therefore not asigned, therefore testing is not necessary
			//CHECK_EQUAL(exp,testId);

			//4) test getMaskAt
			const VoxelGridIndex3D inMask1(2, 1, 4); //corner -> volumeFraction = 0.25
			const VoxelGridIndex3D inMask2(3, 4, 4); //inside ->volumeFraction = 1
			const VoxelGridIndex3D inMask3(4, 5, 4); //side -> volumeFraction = 0.5
			const VoxelGridIndex3D outMask1(7, 5, 4);
			const VoxelGridIndex3D outMask2(2, 1, 2);

			core::MaskVoxel tmpMV1(0), tmpMV2(0);
			CHECK(testOTB2.getMaskAt(inMask1, tmpMV1));
			CHECK(testOTB2.getGeometricInfo().convert(inMask1, testId));
			CHECK(testOTB2.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(0.25, tmpMV1.getRelevantVolumeFraction());
			CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

			CHECK(testOTB2.getMaskAt(inMask2, tmpMV1));
			CHECK(testOTB2.getGeometricInfo().convert(inMask2, testId));
			CHECK(testOTB2.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(1, tmpMV1.getRelevantVolumeFraction());
			CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

			CHECK(testOTB2.getMaskAt(inMask3, tmpMV1));
			CHECK(testOTB2.getGeometricInfo().convert(inMask3, testId));
			CHECK(testOTB2.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(0.5, tmpMV1.getRelevantVolumeFraction());
			CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

			CHECK(!testOTB2.getMaskAt(outMask1, tmpMV1));
			CHECK(testOTB2.getGeometricInfo().convert(outMask1, testId));
			CHECK(!testOTB2.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());
			//CHECK_EQUAL(testId,tmpMV1.getVoxelGridID()); -> return value will not be valid outside the mask

			CHECK(!testOTB2.getMaskAt(outMask2, tmpMV1));
			CHECK(testOTB2.getGeometricInfo().convert(outMask2, testId));
			CHECK(!testOTB2.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());
			//CHECK_EQUAL(testId,tmpMV1.getVoxelGridID()); -> return value will not be valid outside the mask

			//5) other interface functions
			CHECK_EQUAL(true, testOTB1.isGridHomogeneous());
			CHECK_NO_THROW(testOTB1.getMaskUID());
			CHECK(testOTB1.getMaskUID() != testOTB2.getMaskUID());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}//testing
}//rttb

