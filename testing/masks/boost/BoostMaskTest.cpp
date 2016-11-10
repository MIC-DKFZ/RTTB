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
// @version $Revision: 1127 $ (last changed revision)
// @date    $Date: 2015-10-01 13:33:33 +0200 (Do, 01 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"

#include "../../core/DummyStructure.h"
#include "../../core/DummyDoseAccessor.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDVHCalculator.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbBoostMask.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbInvalidParameterException.h"


namespace rttb
{
	namespace testing
	{

		/*! @brief BoostMaskRedesignTest.
			1) test constructors
			2) test getRelevantVoxelVector
			3) test getMaskAt
		*/
		int BoostMaskTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
            typedef core::Structure::StructTypePointer StructTypePointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			// generate test dose. geometric info: patient position (-25, -2, 35), center of the 1st.voxel 
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor =
			    boost::make_shared<DummyDoseAccessor>();
            boost::shared_ptr<core::GeometricInfo> geometricPtr = boost::make_shared<core::GeometricInfo>
                (spTestDoseAccessor->getGeometricInfo());


			DummyStructure myStructGenerator(spTestDoseAccessor->getGeometricInfo());

            //generate test structure. contours are (-20,0.5,38.75); (-12.5,0.5,38.75); (-12.5,10.5,38.75); (-20,10.5,38.75); 
            //(-20, 0.5, 41.25); (-12.5, 0.5, 41.25); (-12.5, 10.5, 41.25); (-20, 10.5, 41.25);
			core::Structure myTestStruct = myStructGenerator.CreateRectangularStructureCentered(2,3);
			StructTypePointer spMyStruct = boost::make_shared<core::Structure>(myTestStruct);
			
            //generate test structure 2. contours are (-20,0.5,38.75); (-12.5,0.5,38.75); (-12.5,10.5,38.75); (-20,10.5,38.75); 
            //(-20, 0.5, 40); (-12.5, 0.5, 40); (-12.5, 10.5, 40); (-20, 10.5, 40);
            core::Structure myTestStruct2 = myStructGenerator.CreateRectangularStructureCenteredContourPlaneThicknessNotEqualDosePlaneThickness(2);
            StructTypePointer spMyStruct2 = boost::make_shared<core::Structure>(myTestStruct2);

			//1) test BoostMask & BoostMaskAccessor constructor
			CHECK_NO_THROW(rttb::masks::boost::BoostMask(geometricPtr, spMyStruct));
			rttb::masks::boost::BoostMask boostMask = rttb::masks::boost::BoostMask(
			            geometricPtr, spMyStruct);

            CHECK_NO_THROW(rttb::masks::boost::BoostMaskAccessor(spMyStruct,
                spTestDoseAccessor->getGeometricInfo(), true));
            rttb::masks::boost::BoostMaskAccessor boostMaskAccessor(spMyStruct,
                spTestDoseAccessor->getGeometricInfo(), true);

            //2) test getRelevantVoxelVector
            CHECK_NO_THROW(boostMask.getRelevantVoxelVector());
            CHECK_NO_THROW(boostMaskAccessor.getRelevantVoxelVector());

            //3) test getMaskAt
            const VoxelGridIndex3D inMask1(2, 1, 2); //corner between two contours slice -> volumeFraction = 0.25 
            const VoxelGridIndex3D inMask2(3, 4, 2); //inside between two contours slice ->volumeFraction = 1 
            const VoxelGridIndex3D inMask3(4, 5, 2); //side between two contours slice -> volumeFraction = 0.5 
            const VoxelGridIndex3D inMask4(2, 1, 1); //corner on the first contour slice -> volumeFraction = 0.25/2 = 0.125
            const VoxelGridIndex3D inMask5(2, 1, 3); //corner on the last contour slice -> volumeFraction = 0.25/2 = 0.125
            const VoxelGridIndex3D inMask6(3, 4, 1); //inside on the first contour slice ->volumeFraction = 1 /2 = 0.5
            const VoxelGridIndex3D outMask1(7, 5, 4);
            const VoxelGridIndex3D outMask2(2, 1, 0);
            const VoxelGridIndex3D outMask3(2, 1, 4);
            VoxelGridID testId;
            double errorConstant = 1e-7;

            core::MaskVoxel tmpMV1(0), tmpMV2(0);
            CHECK(boostMaskAccessor.getMaskAt(inMask1, tmpMV1));
            geometricPtr->convert(inMask1, testId);
            CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_CLOSE(0.25, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor.getMaskAt(inMask2, tmpMV1));
            CHECK(geometricPtr->convert(inMask2, testId));
            CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_EQUAL(1, tmpMV1.getRelevantVolumeFraction());
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor.getMaskAt(inMask3, tmpMV1));
            CHECK(geometricPtr->convert(inMask3, testId));
            CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_CLOSE(0.5, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor.getMaskAt(inMask4, tmpMV1));
            CHECK(geometricPtr->convert(inMask4, testId));
            CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_CLOSE(0.125, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor.getMaskAt(inMask5, tmpMV1));
            CHECK(geometricPtr->convert(inMask5, testId));
            CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_CLOSE(0.125, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor.getMaskAt(inMask6, tmpMV1));
            CHECK(geometricPtr->convert(inMask6, testId));
            CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_CLOSE(0.5, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(!boostMaskAccessor.getMaskAt(outMask1, tmpMV1));
            CHECK(geometricPtr->convert(outMask1, testId));
            CHECK(!boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());

            CHECK(!boostMaskAccessor.getMaskAt(outMask2, tmpMV1));
            CHECK(geometricPtr->convert(outMask2, testId));
            CHECK(!boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());

            CHECK(!boostMaskAccessor.getMaskAt(outMask3, tmpMV1));
            CHECK(geometricPtr->convert(outMask3, testId));
            CHECK(!boostMaskAccessor.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());

            rttb::masks::boost::BoostMask boostMask2 = rttb::masks::boost::BoostMask(
                geometricPtr, spMyStruct2);
            CHECK_NO_THROW(boostMask2.getRelevantVoxelVector());
            rttb::masks::boost::BoostMaskAccessor boostMaskAccessor2(spMyStruct2,
                spTestDoseAccessor->getGeometricInfo(), true);
            CHECK_NO_THROW(boostMaskAccessor2.getRelevantVoxelVector());

            CHECK(boostMaskAccessor2.getMaskAt(inMask1, tmpMV1));
            geometricPtr->convert(inMask1, testId);
            CHECK(boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            //corner, the first contour weight 0.25, the second contour weights 0.5  -> volumeFraction = 0.25*0.25 + 1.25*0.5 = 0.1875 
            CHECK_CLOSE(0.1875, tmpMV1.getRelevantVolumeFraction(), errorConstant); 
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor2.getMaskAt(inMask2, tmpMV1));
            CHECK(geometricPtr->convert(inMask2, testId));
            CHECK(boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            //inside, the first contour weight 0.25, the second contour weights 0.5  -> ->volumeFraction = 1*0.25 + 1*0.5 = 0.75 
            CHECK_EQUAL(0.75, tmpMV1.getRelevantVolumeFraction());
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor2.getMaskAt(inMask3, tmpMV1));
            CHECK(geometricPtr->convert(inMask3, testId));
            CHECK(boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            //side the first contour weight 0.25, the second contour weights 0.5  -> ->volumeFraction = 0.5*0.25 + 0.5*0.5 = 0.75 
            CHECK_CLOSE(0.375, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());			

            CHECK(boostMaskAccessor2.getMaskAt(inMask4, tmpMV1));
            CHECK(geometricPtr->convert(inMask4, testId));
            CHECK(boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            //corner on the first contour slice, weight 0.25 -> volumeFraction = 0.25*0.25 = 0.0625
            CHECK_CLOSE(0.0625, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(boostMaskAccessor2.getMaskAt(inMask6, tmpMV1));
            CHECK(geometricPtr->convert(inMask6, testId));
            CHECK(boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            //inside on the first contour slice, weight 0.25 ->volumeFraction = 1 * 0.25 = 0.25
            CHECK_CLOSE(0.25, tmpMV1.getRelevantVolumeFraction(), errorConstant);
            CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

            CHECK(!boostMaskAccessor2.getMaskAt(outMask1, tmpMV1));
            CHECK(geometricPtr->convert(outMask1, testId));
            CHECK(!boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());
            //CHECK_EQUAL(testId,tmpMV1.getVoxelGridID()); -> return value will not be valid outside the mask

            CHECK(!boostMaskAccessor2.getMaskAt(outMask2, tmpMV1));
            CHECK(geometricPtr->convert(outMask2, testId));
            CHECK(!boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());
            //CHECK_EQUAL(testId,tmpMV1.getVoxelGridID()); -> return value will not be valid outside the mask

            CHECK(!boostMaskAccessor2.getMaskAt(outMask3, tmpMV1));
            CHECK(geometricPtr->convert(outMask3, testId));
            CHECK(!boostMaskAccessor2.getMaskAt(testId, tmpMV2));
            CHECK_EQUAL(tmpMV1, tmpMV2);
            CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());
            //CHECK_EQUAL(testId,tmpMV1.getVoxelGridID()); -> return value will not be valid outside the mask

            RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}//testing
}//rttb

