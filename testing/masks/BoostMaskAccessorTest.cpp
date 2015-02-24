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
// @version $Revision: 880 $ (last changed revision)
// @date    $Date: 2015-01-13 14:14:24 +0100 (Di, 13 Jan 2015) $ (last change date)
// @author  $Author: zhangl $ (last changed by)
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
#include "rttbBoostMask.h"


namespace rttb
{
	namespace testing
	{

		/*! @brief BoostMaskAccessorTest.
			1) test constructors
			2) test updateMask (do relevant voxels match?)
			3) test valid/convert
			4) test getMaskAt
			5) other interface functions (simple getters)
		*/
		int BoostMaskAccessorTest(int argc, char* argv[])
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

			//test BoostMask constructor
			boost::shared_ptr<core::GeometricInfo> geometricPtr = boost::make_shared<core::GeometricInfo>(spTestDoseAccessor->getGeometricInfo());
			

			CHECK_NO_THROW( rttb::masks::BoostMask(geometricPtr, spMyStruct));
			rttb::masks::BoostMask boostMask = rttb::masks::BoostMask(geometricPtr, spMyStruct);

			
			//2) test getRelevantVoxelVector 
			CHECK_NO_THROW(boostMask.getRelevantVoxelVector());

			MaskVoxelListPointer voxelListBoost = boostMask.getRelevantVoxelVector();


			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}//testing
}//rttb

