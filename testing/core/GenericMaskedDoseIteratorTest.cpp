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
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbNullPointerException.h"
#include "rttbException.h"
#include "DummyDoseAccessor.h"
#include "DummyMaskAccessor.h"

namespace rttb
{
	namespace testing
	{

		typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
		typedef core::GenericMaskedDoseIterator::DoseAccessorPointer DoseAccessorPointer;

		/*! @brief GenericMaskedDoseIteratorTest.
			1) test constructor (values as expected?)
			2) test reset/next/get current values/isPositionValid
		*/
		int GenericMaskedDoseIteratorTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor = boost::make_shared<DummyDoseAccessor>();
			DoseAccessorPointer spDoseAccessor(spTestDoseAccessor);
			const std::vector<DoseTypeGy>* doseVals = spTestDoseAccessor->getDoseVector();

			core::GeometricInfo geoInfo;
			boost::shared_ptr<DummyMaskAccessor> spTestMaskAccessor = boost::make_shared<DummyMaskAccessor>
			        (geoInfo);
			MaskAccessorPointer spMaskAccessor(spTestMaskAccessor);

			//1) test constructor (values as expected?)
			//not NULL pointer
			MaskAccessorPointer spNullMaskAccessor;
			DoseAccessorPointer spNullDoseAccessor;
			CHECK_THROW_EXPLICIT(core::GenericMaskedDoseIterator genMaskedDoseIterator(spNullMaskAccessor,
			                     spDoseAccessor),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(core::GenericMaskedDoseIterator genMaskedDoseIterator(spMaskAccessor,
			                     spNullDoseAccessor),
			                     core::NullPointerException);
			//not same core::GeometricInfo
			CHECK_THROW_EXPLICIT(core::GenericMaskedDoseIterator genMaskedDoseIterator(spMaskAccessor,
			                     spDoseAccessor), core::Exception);
			//set correct GeometricInfo
			geoInfo = spDoseAccessor->getGeometricInfo();
			boost::shared_ptr<DummyMaskAccessor> spTestMaskAccessor1 = boost::make_shared<DummyMaskAccessor>
			        (geoInfo);
			MaskAccessorPointer spMaskAccessorTemp(spTestMaskAccessor1);
			spMaskAccessor.swap(spMaskAccessorTemp);
			CHECK_NO_THROW(core::GenericMaskedDoseIterator genMaskedDoseIterator(spMaskAccessor,
			               spDoseAccessor));
      CHECK_EQUAL(spMaskAccessor->isGridHomogeneous(), true);
			core::GenericMaskedDoseIterator genMaskedDoseIterator(spMaskAccessor, spDoseAccessor);

			//2) test reset/next
			const DummyMaskAccessor::MaskVoxelListPointer maskedVoxelListPtr =
			    spTestMaskAccessor1->getRelevantVoxelVector();
			genMaskedDoseIterator.reset();
			const DoseVoxelVolumeType homogeneousVoxelVolume = genMaskedDoseIterator.getCurrentVoxelVolume();
			CHECK_EQUAL((maskedVoxelListPtr->begin())->getVoxelGridID(),
			            genMaskedDoseIterator.getCurrentVoxelGridID());
			geoInfo = spDoseAccessor->getGeometricInfo();
			SpacingVectorType3D spacing = geoInfo.getSpacing();
			CHECK_EQUAL(spacing(0)*spacing(1)*spacing(2) / 1000, genMaskedDoseIterator.getCurrentVoxelVolume());

			//check if the correct voxels are accessed
			genMaskedDoseIterator.reset();
			VoxelGridID defaultDoseVoxelGridID = genMaskedDoseIterator.getCurrentVoxelGridID();
			DoseTypeGy controlValue = 0;
			VoxelGridID position = 0;

			while (genMaskedDoseIterator.isPositionValid())
			{
				controlValue = doseVals->at((maskedVoxelListPtr->at(position)).getVoxelGridID());
				CHECK_EQUAL(controlValue, genMaskedDoseIterator.getCurrentDoseValue());
				controlValue = controlValue * (maskedVoxelListPtr->at(position)).getRelevantVolumeFraction();
				CHECK_EQUAL(controlValue, genMaskedDoseIterator.getCurrentMaskedDoseValue());
				CHECK_EQUAL(homogeneousVoxelVolume, genMaskedDoseIterator.getCurrentVoxelVolume());
				CHECK_EQUAL((maskedVoxelListPtr->at(position)).getRelevantVolumeFraction(),
				            genMaskedDoseIterator.getCurrentRelevantVolumeFraction());
				CHECK_EQUAL((maskedVoxelListPtr->at(position)).getVoxelGridID(),
				            genMaskedDoseIterator.getCurrentVoxelGridID());
				CHECK(genMaskedDoseIterator.isPositionValid());

				genMaskedDoseIterator.next();
				position++;
			}

			//check isPositionValid() in invalid positions
			CHECK(!(genMaskedDoseIterator.isPositionValid())); //after end of dose
			genMaskedDoseIterator.reset();
			CHECK_EQUAL(defaultDoseVoxelGridID, genMaskedDoseIterator.getCurrentVoxelGridID());
			CHECK(genMaskedDoseIterator.isPositionValid());//at start of dose

			RETURN_AND_REPORT_TEST_SUCCESS;
		}



	}//testing
}//rttb

