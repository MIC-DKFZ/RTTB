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
#include "rttbDoseAccessorInterface.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbArithmetic.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbBinaryFunctorAccessor.h"

namespace rttb
{
	namespace testing
	{
		typedef core::DoseAccessorInterface::Pointer DoseAccessorPointer;
		typedef  algorithms::BinaryFunctorAccessor<algorithms::arithmetic::doseOp::Add>
		BinaryFunctorAccessorAddType;
		typedef  algorithms::BinaryFunctorAccessor<algorithms::arithmetic::doseOp::AddWeighted>
		BinaryFunctorAccessorAddWeightedType;

		/*! @brief BinaryFunctorAccessorTest - tests functors of two accessors
				1) test constructor
				2) test getDoseAt
			*/

		int BinaryFunctorAccessorTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE2_FILENAME = argv[2];
			}


			DoseAccessorPointer spDoseAccessorNull;

			DoseAccessorPointer spDoseAccessor = io::dicom::DicomFileDoseAccessorGenerator(
			        RTDOSE_FILENAME.c_str()).generateDoseAccessor();
			DoseAccessorPointer spDoseAccessor2 = io::dicom::DicomFileDoseAccessorGenerator(
			        RTDOSE2_FILENAME.c_str()).generateDoseAccessor();

			algorithms::arithmetic::doseOp::Add addOP;

			algorithms::arithmetic::doseOp::AddWeighted addWeightedOP(1.0, 10.0);
			algorithms::arithmetic::doseOp::AddWeighted addWeightedTwoOP(2.0, 2.0);


			//1) Check constructor

			CHECK_THROW_EXPLICIT(BinaryFunctorAccessorAddType(spDoseAccessorNull, spDoseAccessor, addOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(BinaryFunctorAccessorAddType(spDoseAccessor, spDoseAccessorNull, addOP),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(BinaryFunctorAccessorAddType(spDoseAccessorNull, spDoseAccessorNull,
			                     addOP), core::NullPointerException);
			CHECK_THROW_EXPLICIT(BinaryFunctorAccessorAddType(spDoseAccessor, spDoseAccessor2, addOP),
			                     core::InvalidParameterException);


			CHECK_NO_THROW(BinaryFunctorAccessorAddType(spDoseAccessor, spDoseAccessor, addOP));
			CHECK_NO_THROW(BinaryFunctorAccessorAddWeightedType(spDoseAccessor, spDoseAccessor,
			               addWeightedOP));

			auto spBinaryFunctorDoseAccessorAdd = boost::make_shared<BinaryFunctorAccessorAddType>(spDoseAccessor, spDoseAccessor, addOP);
			auto spBinaryFunctorDoseAccessorAddWeighted = boost::make_shared<BinaryFunctorAccessorAddWeightedType>(spDoseAccessor, spDoseAccessor, addWeightedOP);
			auto spBinaryFunctorDoseAccessorAddWeightedTwo = boost::make_shared<BinaryFunctorAccessorAddWeightedType>(spDoseAccessor, spDoseAccessor, addWeightedTwoOP);

			//2) Test getDoseAt()
			int lastIndex = spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumberOfVoxels() - 1;
			VoxelGridID aId[3] = { 5, 6067, lastIndex };
			VoxelGridIndex3D aIndex[3] = {VoxelGridIndex3D(5, 0, 0), VoxelGridIndex3D(37, 0, 2), VoxelGridIndex3D(spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumColumns() - 1, spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumRows() - 1, spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumSlices() - 1)};

			for (int i = 0; i < 3; ++i)
			{
				CHECK_EQUAL(spBinaryFunctorDoseAccessorAdd->getValueAt(aId[i]), 4.0);
				CHECK_EQUAL(spBinaryFunctorDoseAccessorAddWeighted->getValueAt(aId[i]), 22.0);
				CHECK_EQUAL(spBinaryFunctorDoseAccessorAdd->getValueAt(aIndex[i]),
				            spBinaryFunctorDoseAccessorAdd->getValueAt(aId[i]));
				CHECK_EQUAL(spBinaryFunctorDoseAccessorAddWeighted->getValueAt(aIndex[i]),
				            spBinaryFunctorDoseAccessorAddWeighted->getValueAt(aId[i]));
				CHECK_EQUAL(spBinaryFunctorDoseAccessorAdd->getValueAt(aId[i]) * 2.0,
				            spBinaryFunctorDoseAccessorAddWeightedTwo->getValueAt(aId[i]));
			}

			VoxelGridID aIdInvalid(spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumberOfVoxels());
			VoxelGridIndex3D aIndexInvalid(spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumColumns(),
			                               spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumRows(),
			                               spBinaryFunctorDoseAccessorAdd->getGeometricInfo().getNumSlices());
			CHECK_EQUAL(spBinaryFunctorDoseAccessorAdd->getValueAt(aIdInvalid), -1.0);
			CHECK_EQUAL(spBinaryFunctorDoseAccessorAdd->getValueAt(aIndexInvalid), -1.0);
			CHECK_EQUAL(spBinaryFunctorDoseAccessorAddWeighted->getValueAt(aIdInvalid), -1.0);
			CHECK_EQUAL(spBinaryFunctorDoseAccessorAddWeighted->getValueAt(aIndexInvalid), -1.0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}
}