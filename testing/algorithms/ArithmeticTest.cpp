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
#include "../core/DummyDoseAccessor.h"
#include "../core/DummyMaskAccessor.h"
#include "../core/DummyMutableDoseAccessor.h"
#include "rttbDoseAccessorInterface.h"
#include "rttbMutableDoseAccessorInterface.h"
#include "rttbMutableMaskAccessorInterface.h"
#include "rttbArithmetic.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbGenericMutableMaskAccessor.h"
#include "rttbMaskAccessorInterface.h"

namespace rttb
{
	namespace testing
	{

		typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;
		typedef core::MutableDoseAccessorInterface::MutableDoseAccessorPointer MutableDoseAccessorPointer;
		typedef DummyMaskAccessor::MaskVoxelListPointer MaskVoxelListPointer;
		typedef DummyMaskAccessor::MaskVoxelList MaskVoxelList;
		typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;
		typedef core::MutableMaskAccessorInterface::MutableMaskAccessorPointer MutableMaskAccessorPointer;

		/*! @brief ArithmeticTest - tests arithmetic combinations of accessors
				1) test dose-dose operations
				2) test dose-mask operations
				3) test mask-mask operations
		  4) test convenience functions
			*/

		int ArithmeticTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;
			// initialize accessors for arithmetic

			//DOSE:
			//Null pointers
			DoseAccessorPointer spDoseNull;
			MutableDoseAccessorPointer spMutableDoseNull;
			//dose with random values between 0 and 100
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor1 = boost::make_shared<DummyDoseAccessor>();
			DoseAccessorPointer spDoseAccessor1(spTestDoseAccessor1);
			//generate a 2nd dose with fixed values
			core::GeometricInfo geoInfo = spDoseAccessor1->getGeometricInfo();

			DoseTypeGy valFix = 101; // to ensure values are larger than 100 on adding
			std::vector<DoseTypeGy> dose2Vals(geoInfo.getNumberOfVoxels(), valFix);

			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor2 = boost::make_shared<DummyDoseAccessor>
			        (dose2Vals, geoInfo);
			DoseAccessorPointer spDoseAccessor2(spTestDoseAccessor2);

			//generate result acessor
			std::vector<DoseTypeGy> doseResultVals(geoInfo.getNumberOfVoxels(), -1);
			boost::shared_ptr<DummyMutableDoseAccessor> spTestMutableDoseAccessor =
			    boost::make_shared<DummyMutableDoseAccessor>(doseResultVals, geoInfo);
			MutableDoseAccessorPointer spMutableDoseAccessor(spTestMutableDoseAccessor);

			// different geometricInfo
			core::GeometricInfo geoInfo2 = geoInfo;
			geoInfo2.setNumColumns(5);
			geoInfo2.setNumRows(5);
			geoInfo2.setNumSlices(5);
			std::vector<DoseTypeGy> dose3Vals(geoInfo2.getNumberOfVoxels(), valFix);
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor3 = boost::make_shared<DummyDoseAccessor>
			        (dose3Vals, geoInfo2);
			DoseAccessorPointer spDoseAccessorDiffGeoInfo(spTestDoseAccessor3);

			boost::shared_ptr<DummyMutableDoseAccessor> spTestMutableDoseAccessor2 =
			    boost::make_shared<DummyMutableDoseAccessor>(dose3Vals, geoInfo2);
			MutableDoseAccessorPointer spMutableDoseAccessorDiffGeoInfo(spTestMutableDoseAccessor2);


			//MASK:
			//null pointer
			MaskAccessorPointer spMaskAccessorNull;
			MutableMaskAccessorPointer spMutableMaskAccessorNull;

			MaskVoxelList voxelList;
			FractionType aVolumeFraction = 1;
			VoxelGridID aVoxelGridID = 10;

			//generate a dummy mask
			while (aVoxelGridID < geoInfo.getNumberOfVoxels() && aVoxelGridID <= 30)
			{
				voxelList.push_back(core::MaskVoxel(aVoxelGridID, aVolumeFraction));
				++aVoxelGridID;
			}

			MaskVoxelListPointer voxelListPtr = boost::make_shared<MaskVoxelList>(voxelList);
			boost::shared_ptr<DummyMaskAccessor> dummyMask1 = boost::make_shared<DummyMaskAccessor>(geoInfo,
			        voxelListPtr);
			MaskAccessorPointer spMaskAccessor1(dummyMask1);

			MaskVoxelList voxelList2;
			aVoxelGridID = 20;

			//generate a 2nd dummy mask that partly overlaps with the 1st one
			while (aVoxelGridID < geoInfo.getNumberOfVoxels() && aVoxelGridID <= 40)
			{
				voxelList2.push_back(core::MaskVoxel(aVoxelGridID, aVolumeFraction));
				++aVoxelGridID;
			}

			MaskVoxelListPointer voxelListPtr2 = boost::make_shared<MaskVoxelList>(voxelList2);
			boost::shared_ptr<DummyMaskAccessor> dummyMask2 = boost::make_shared<DummyMaskAccessor>(geoInfo,
			        voxelListPtr2);
			MaskAccessorPointer spMaskAccessor2(dummyMask2);
			// result accessor
			boost::shared_ptr<masks::GenericMutableMaskAccessor> mMask1 =
			    boost::make_shared<masks::GenericMutableMaskAccessor>(geoInfo);
			MutableMaskAccessorPointer spMutableMask(mMask1);

			// different geometricInfo
			boost::shared_ptr<DummyMaskAccessor> spDummyMaskDiffGeoInfo = boost::make_shared<DummyMaskAccessor>
			        (geoInfo2, voxelListPtr2);
			MaskAccessorPointer spMaskAccessorDiffGeoInfo(spDummyMaskDiffGeoInfo);
			boost::shared_ptr<masks::GenericMutableMaskAccessor> mMask2 =
			    boost::make_shared<masks::GenericMutableMaskAccessor>(geoInfo2);
			MutableMaskAccessorPointer spMutableMaskDiffGeoInfo(mMask2);

			// 1) test dose-dose operations
			//ADD
			algorithms::arithmetic::doseOp::Add addOP;
			CHECK_NO_THROW(algorithms::arithmetic::arithmetic(spDoseAccessor1, spDoseAccessor2,
			               spMutableDoseAccessor, addOP));
			VoxelGridID id = 5;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 100);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) + valFix, spMutableDoseAccessor->getDoseAt(id));
			id = 10;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 100);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) + valFix, spMutableDoseAccessor->getDoseAt(id));
			//@todo add MappableDoseAccessor Tests


			//handling exceptions is tested once for dose-dose operations, because this does not change if the operation changes.
			//handling null pointers
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor1, spDoseNull,
			                     spMutableDoseAccessor, addOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseNull, spDoseAccessor2,
			                     spMutableDoseAccessor, addOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor1, spDoseAccessor2,
			                     spMutableDoseNull, addOP),
			                     core::NullPointerException);
			//handle different geometricInfos
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor1, spDoseAccessorDiffGeoInfo,
			                     spMutableDoseAccessor,
			                     addOP), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessorDiffGeoInfo, spDoseAccessor2,
			                     spMutableDoseAccessor,
			                     addOP), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor1, spDoseAccessor2,
			                     spMutableDoseAccessorDiffGeoInfo,
			                     addOP), core::InvalidParameterException);

			//ADD_WEIGHTED
			algorithms::arithmetic::doseOp::AddWeighted addWOP(1, 2);
			CHECK_NO_THROW(algorithms::arithmetic::arithmetic(spDoseAccessor1, spDoseAccessor2,
			               spMutableDoseAccessor, addWOP));
			id = 5;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 201);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) + 2 * valFix, spMutableDoseAccessor->getDoseAt(id));
			id = 10;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 201);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) + 2 * valFix, spMutableDoseAccessor->getDoseAt(id));

			//MULTIPLY
			algorithms::arithmetic::doseOp::Multiply multiplyOP;
			CHECK_NO_THROW(algorithms::arithmetic::arithmetic(spDoseAccessor1, spDoseAccessor2,
			               spMutableDoseAccessor, multiplyOP));
			id = 5;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 201);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) * valFix, spMutableDoseAccessor->getDoseAt(id));
			id = 10;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 201);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) * valFix, spMutableDoseAccessor->getDoseAt(id));
			//@todo add MappableDoseAccessor Tests

			// 2) test dose-mask operations
			//MULTIPLY
			algorithms::arithmetic::doseMaskOp::Multiply multOP;
			CHECK_NO_THROW(algorithms::arithmetic::arithmetic(spDoseAccessor2, spMaskAccessor1,
			               spMutableDoseAccessor, multOP));

			core::MaskVoxel mVoxel(0);
			id = 5;
			CHECK_EQUAL(0, spMutableDoseAccessor->getDoseAt(id));
			spMaskAccessor1->getMaskAt(id, mVoxel);
			CHECK_EQUAL(spMutableDoseAccessor->getDoseAt(id), mVoxel.getRelevantVolumeFraction());
			id = 15;
			CHECK_EQUAL(valFix, spMutableDoseAccessor->getDoseAt(id));
			id = 35;
			CHECK_EQUAL(0, spMutableDoseAccessor->getDoseAt(id));
			//@todo add MappableDoseAccessor Tests

			//handling exceptions is tested once for dose-dose operations, because this does not change if the operation changes.
			//handling null pointers
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor1, spMaskAccessorNull,
			                     spMutableDoseAccessor, multOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseNull, spDoseAccessor2,
			                     spMutableDoseAccessor, multOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor2, spMaskAccessor1,
			                     spMutableDoseNull, multOP),
			                     core::NullPointerException);
			//handle different geometricInfos
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor2, spMaskAccessorDiffGeoInfo,
			                     spMutableDoseAccessor,
			                     multOP), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessorDiffGeoInfo, spMaskAccessor1,
			                     spMutableDoseAccessor,
			                     multOP), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spDoseAccessor2, spMaskAccessor1,
			                     spMutableDoseAccessorDiffGeoInfo,
			                     multOP), core::InvalidParameterException);

			// 3) test mask-mask operations
			//ADD
			algorithms::arithmetic::maskOp::Add maskAddOP;
			CHECK_NO_THROW(algorithms::arithmetic::arithmetic(spMaskAccessor1, spMaskAccessor2, spMutableMask,
			               maskAddOP));

			id = 5;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());
			id = 15;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(1, mVoxel.getRelevantVolumeFraction());
			id = 35;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(1, mVoxel.getRelevantVolumeFraction());
			id = 45;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());

			//handling exceptions is tested once for dose-dose operations, because this does not change if the operation changes.
			//handling null pointers
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spMaskAccessor1, spMaskAccessorNull,
			                     spMutableMask, maskAddOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spMaskAccessorNull, spMaskAccessor2,
			                     spMutableMask, maskAddOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spMaskAccessor1, spMaskAccessor1,
			                     spMutableMaskAccessorNull, maskAddOP),
			                     core::NullPointerException);
			//handle different geometricInfos
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spMaskAccessor1, spMaskAccessorDiffGeoInfo,
			                     spMutableMask,
			                     maskAddOP), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spMaskAccessorDiffGeoInfo, spMaskAccessor2,
			                     spMutableMask,
			                     maskAddOP), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(algorithms::arithmetic::arithmetic(spMaskAccessor1, spMaskAccessor1,
			                     spMutableMaskDiffGeoInfo,
			                     maskAddOP), core::InvalidParameterException);

			//SUBTRACT
			algorithms::arithmetic::maskOp::Subtract maskSubOP;
			CHECK_NO_THROW(algorithms::arithmetic::arithmetic(spMaskAccessor1, spMaskAccessor2, spMutableMask,
			               maskSubOP));

			id = 5;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());
			id = 15;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(1, mVoxel.getRelevantVolumeFraction());
			id = 35;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());
			id = 45;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());

			// 4) test convenience functions
			// tests are similar to explicit calls
			//@todo if convenience functions are implemented: add MappableDoseAccessor Tests
			CHECK_NO_THROW(algorithms::arithmetic::add(spDoseAccessor1, spDoseAccessor2,
			               spMutableDoseAccessor));
			id = 5;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 100);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) + valFix, spMutableDoseAccessor->getDoseAt(id));
			id = 10;
			CHECK(spMutableDoseAccessor->getDoseAt(id) > 100);
			CHECK_EQUAL(spDoseAccessor1->getDoseAt(id) + valFix, spMutableDoseAccessor->getDoseAt(id));

			CHECK_NO_THROW(algorithms::arithmetic::add(spMaskAccessor1, spMaskAccessor2, spMutableMask));
			id = 5;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());
			id = 15;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(1, mVoxel.getRelevantVolumeFraction());
			id = 35;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(1, mVoxel.getRelevantVolumeFraction());
			id = 45;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());

			CHECK_NO_THROW(algorithms::arithmetic::subtract(spMaskAccessor1, spMaskAccessor2, spMutableMask));
			id = 5;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());
			id = 15;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(1, mVoxel.getRelevantVolumeFraction());
			id = 35;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());
			id = 45;
			spMutableMask->getMaskAt(id, mVoxel);
			CHECK_EQUAL(0, mVoxel.getRelevantVolumeFraction());


			CHECK_NO_THROW(algorithms::arithmetic::multiply(spDoseAccessor2, spMaskAccessor1,
			               spMutableDoseAccessor));
			id = 5;
			CHECK_EQUAL(0, spMutableDoseAccessor->getDoseAt(id));
			spMaskAccessor1->getMaskAt(id, mVoxel);
			CHECK_EQUAL(spMutableDoseAccessor->getDoseAt(id), mVoxel.getRelevantVolumeFraction());
			id = 15;
			CHECK_EQUAL(valFix, spMutableDoseAccessor->getDoseAt(id));
			id = 35;
			CHECK_EQUAL(0, spMutableDoseAccessor->getDoseAt(id));


			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}
}