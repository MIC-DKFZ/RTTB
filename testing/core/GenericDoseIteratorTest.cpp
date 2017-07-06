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

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGenericDoseIterator.h"
#include "DummyDoseAccessor.h"

namespace rttb
{
	namespace testing
	{

		typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;

		/*! @brief GenericDoseIteratorTest - test the API of GenericDoseIterator
			1) test constructor (values as expected?)
			2) test reset/next/get current values/isPositionValid
      3) test DoseIteratorInterface functions
		*/
		int GenericDoseIteratorTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//create dummy DoseAccessor
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor = boost::make_shared<DummyDoseAccessor>();
			DoseAccessorPointer spDoseAccessor(spTestDoseAccessor);

			//1) test constructor (values as expected?)
			CHECK_NO_THROW(core::GenericDoseIterator genDoseIterator(spDoseAccessor));
			core::GenericDoseIterator genDoseIterator(spDoseAccessor);
			const VoxelGridID defaultDoseVoxelGridID = 0;
			const DoseVoxelVolumeType defaultVoxelVolume = 0;
			CHECK_EQUAL(defaultDoseVoxelGridID, genDoseIterator.getCurrentVoxelGridID());
			CHECK_EQUAL(defaultVoxelVolume, genDoseIterator.getCurrentVoxelVolume());

			//2) test reset/next
			genDoseIterator.reset();
			const DoseVoxelVolumeType homogeneousVoxelVolume = genDoseIterator.getCurrentVoxelVolume();
			CHECK_EQUAL(defaultDoseVoxelGridID, genDoseIterator.getCurrentVoxelGridID());
			CHECK(!(defaultVoxelVolume == genDoseIterator.getCurrentVoxelVolume()));
			core::GeometricInfo geoInfo = spTestDoseAccessor->getGeometricInfo();
			SpacingVectorType3D spacing = geoInfo.getSpacing();
			CHECK_EQUAL(spacing(0)*spacing(1)*spacing(2) / 1000, genDoseIterator.getCurrentVoxelVolume());

			//check if the correct voxels are accessed
			const std::vector<DoseTypeGy>* doseVals = spTestDoseAccessor->getDoseVector();
			genDoseIterator.reset();
			VoxelGridID position = 0;

			while (genDoseIterator.isPositionValid())
			{
				CHECK_EQUAL(genDoseIterator.getCurrentDoseValue(), doseVals->at(position));
				CHECK_EQUAL(homogeneousVoxelVolume, genDoseIterator.getCurrentVoxelVolume());
				CHECK_EQUAL(1, genDoseIterator.getCurrentRelevantVolumeFraction());
				CHECK_EQUAL(position, genDoseIterator.getCurrentVoxelGridID());

				genDoseIterator.next();
				position++;
			}

			//check isPositionValid() in invalid positions
			CHECK(!(genDoseIterator.isPositionValid())); //after end of dose
			genDoseIterator.reset();
			CHECK_EQUAL(defaultDoseVoxelGridID, genDoseIterator.getCurrentVoxelGridID());
			CHECK(genDoseIterator.isPositionValid());//before start of dose

      //3) test DoseIteratorInterface functions
      CHECK_EQUAL(genDoseIterator.getVoxelizationID(), "");
      CHECK_EQUAL(genDoseIterator.getDoseUID(), spTestDoseAccessor->getUID());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb

