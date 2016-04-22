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
#include "rttbBoostMaskRedesign.h"
#include "rttbBoostMask.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbBoostMaskRedesignAccessor.h"


namespace rttb
{
	namespace testing
	{

		/*! @brief BoostMaskRedesignTest.
			1) test constructors
			2) test getRelevantVoxelVector
			3) test getMaskAt
		*/
		int BoostMaskRedesignTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			typedef core::Structure::StructTypePointer StructTypePointer;

			// generate test structure set
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor =
			    boost::make_shared<DummyDoseAccessor>();

			DummyStructure myStructGenerator(spTestDoseAccessor->getGeometricInfo());

			GridIndexType zPlane = 4;
			core::Structure myTestStruct = myStructGenerator.CreateRectangularStructureCentered(zPlane);
			StructTypePointer spMyStruct = boost::make_shared<core::Structure>(myTestStruct);
			boost::shared_ptr<core::GeometricInfo> geometricPtr = boost::make_shared<core::GeometricInfo>
			        (spTestDoseAccessor->getGeometricInfo());

			//1) test BoostMask constructor
			CHECK_NO_THROW(rttb::masks::boostRedesign::BoostMask(geometricPtr, spMyStruct));
			rttb::masks::boostRedesign::BoostMask boostMask = rttb::masks::boostRedesign::BoostMask(
			            geometricPtr, spMyStruct);

			//2) test getRelevantVoxelVector
			CHECK_NO_THROW(boostMask.getRelevantVoxelVector());
			rttb::masks::boostRedesign::BoostMask::MaskVoxelListPointer list =
			    boostMask.getRelevantVoxelVector();

			for (int i = 0; i < list->size(); ++i)
			{
				std::cout << "id: " << list->at(i).getVoxelGridID() << ", " << list->at(
				              i).getRelevantVolumeFraction() << std::endl;
			}


			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}//testing
}//rttb

