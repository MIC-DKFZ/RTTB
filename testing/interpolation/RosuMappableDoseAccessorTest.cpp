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

#include <boost/make_shared.hpp>

#include "rttbBaseType.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbRosuMappableDoseAccessor.h"

#include "DummyTransformation.h"

#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"

namespace rttb
{
	namespace testing
	{
		typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
		typedef rttb::interpolation::RosuMappableDoseAccessor RosuMappableDoseAccessor;
		typedef rttb::interpolation::TransformationInterface TransformationInterface;

		/*! @brief RosuMappableDoseAccessorTest - test the API of RosuMappableDoseAccessor
			1) Constructor
			2) test getDoseAt()
		*/

		int RosuMappableDoseAccessorTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string RTDOSE_FILENAME_CONSTANT_TWO;
			std::string RTDOSE_FILENAME_INCREASE_X;

			if (argc > 2)
			{
				RTDOSE_FILENAME_CONSTANT_TWO = argv[1];
				RTDOSE_FILENAME_INCREASE_X = argv[2];
			}
			else
			{
				std::cout << "at least two parameters for RosuMappableDoseAccessorTest are expected" << std::endl;
				return -1;
			}

			const double doseGridScaling = 2.822386e-005;

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(
			    RTDOSE_FILENAME_CONSTANT_TWO.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(
			    RTDOSE_FILENAME_INCREASE_X.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			DoseAccessorPointer doseAccessorNull;

      auto transformDummy = boost::make_shared<DummyTransformation>();
			TransformationInterface::Pointer transformNull;

      auto aRosuMappableDoseAccessorDefault = boost::make_shared<RosuMappableDoseAccessor>(doseAccessor1->getGeometricInfo(), doseAccessor2, transformDummy);
			auto aRosuMappableDoseAccessorNoPadding = boost::make_shared<RosuMappableDoseAccessor>(doseAccessor1->getGeometricInfo(), doseAccessor2, transformDummy, false);

			//1) Constructor

			CHECK_NO_THROW(RosuMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformDummy));
			CHECK_NO_THROW(RosuMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformDummy, false));
			CHECK_NO_THROW(RosuMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformDummy, true, 5.0));

			CHECK_THROW_EXPLICIT(RosuMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessorNull, transformDummy), core::NullPointerException);
			CHECK_THROW_EXPLICIT(RosuMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessor2, transformNull), core::NullPointerException);
			CHECK_THROW_EXPLICIT(RosuMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessorNull, transformNull), core::NullPointerException);

			//2) test getDoseAt()

			std::vector<VoxelGridIndex3D> voxelsAsIndexToTest;
			std::vector<VoxelGridID> voxelsAsIdToTest;
			std::vector<DoseTypeGy> expectedValues;

			voxelsAsIndexToTest.push_back(VoxelGridIndex3D(5, 9, 8));
			voxelsAsIndexToTest.push_back(VoxelGridIndex3D(22, 15, 10));
			voxelsAsIndexToTest.push_back(VoxelGridIndex3D(30, 20, 7));

			expectedValues.push_back(5.0 * doseGridScaling);
			expectedValues.push_back(22.0 * doseGridScaling);
			expectedValues.push_back(30.0 * doseGridScaling);

			//convert VoxelGridIndex3D to VoxelGridID
			for (int i = 0; i < voxelsAsIndexToTest.size(); i++)
			{
				VoxelGridID currentId;
				doseAccessor1->getGeometricInfo().convert(voxelsAsIndexToTest.at(i), currentId);
				voxelsAsIdToTest.push_back(currentId);
			}

			for (int i = 0; i < voxelsAsIndexToTest.size(); i++)
			{
				//test if the expected interpolation values are computed
				CHECK_CLOSE(aRosuMappableDoseAccessorDefault->getValueAt(voxelsAsIndexToTest.at(i)),
				            expectedValues.at(i), errorConstant);
				//test if getDoseAt(VoxelGridIndex3D) and getDoseAt(VoxelGridD) lead to the same results
				CHECK_EQUAL(aRosuMappableDoseAccessorDefault->getValueAt(voxelsAsIndexToTest.at(i)),
				            aRosuMappableDoseAccessorDefault->getValueAt(voxelsAsIdToTest.at(i)));
			}

			//test invalid voxels
			VoxelGridID invalidID(doseAccessor1->getGeometricInfo().getNumberOfVoxels() + 1);
			VoxelGridIndex3D invalidIndex(doseAccessor1->getGeometricInfo().getNumColumns() + 1,
			                              doseAccessor1->getGeometricInfo().getNumRows() + 1,
			                              doseAccessor1->getGeometricInfo().getNumSlices() + 1);
			CHECK_EQUAL(aRosuMappableDoseAccessorDefault->getValueAt(invalidID), 0.0);
			CHECK_EQUAL(aRosuMappableDoseAccessorDefault->getValueAt(invalidIndex), 0.0);
			CHECK_THROW_EXPLICIT(aRosuMappableDoseAccessorNoPadding->getValueAt(invalidID),
			                     core::MappingOutsideOfImageException);
			CHECK_THROW_EXPLICIT(aRosuMappableDoseAccessorNoPadding->getValueAt(invalidIndex),
			                     core::MappingOutsideOfImageException);


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
