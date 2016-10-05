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

#include "rttbBaseType.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbLinearInterpolation.h"
#include "rttbTransformationInterface.h"
#include "DummyTransformation.h"

#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"

namespace rttb
{
	namespace testing
	{
		typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
		typedef rttb::interpolation::SimpleMappableDoseAccessor SimpleMappableDoseAccessor;
		typedef rttb::interpolation::TransformationInterface TransformationInterface;
		typedef rttb::interpolation::LinearInterpolation LinearInterpolation;
		typedef rttb::interpolation::NearestNeighborInterpolation NearestNeighborInterpolation;

		/*! @brief SimpleMappableDoseAccessorTest - test the API of SimpleMappableDoseAccessor
			1) Test constructor
			2) test getDoseAt()
		*/

		int SimpleMappableDoseAccessorTest(int argc, char* argv[])
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
				std::cout << "at least two parameters for SimpleMappableDoseAccessorTest are expected" << std::endl;
				return -1;
			}

			const double doseGridScaling = 2.822386e-005;

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(
			    RTDOSE_FILENAME_CONSTANT_TWO.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());
			core::GeometricInfo doseAccessor1GeometricInfo = doseAccessor1->getGeometricInfo();

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(
			    RTDOSE_FILENAME_INCREASE_X.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			DoseAccessorPointer doseAccessorNull;

            TransformationInterface::Pointer transformDummy = boost::make_shared<DummyTransformation>();
			TransformationInterface::Pointer transformNull;

            NearestNeighborInterpolation::Pointer interpolationNN = boost::make_shared<NearestNeighborInterpolation>();
            LinearInterpolation::Pointer interpolationLinear = boost::make_shared<LinearInterpolation>();
			boost::shared_ptr<NearestNeighborInterpolation> interpolationNull;

            SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorDefault = boost::make_shared<SimpleMappableDoseAccessor>(
                doseAccessor1->getGeometricInfo(), doseAccessor2, transformDummy);
            SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorNoPadding = boost::make_shared<SimpleMappableDoseAccessor>(
                doseAccessor1->getGeometricInfo(), doseAccessor2, transformDummy, interpolationLinear, false);

			//1) Test constructor

            CHECK_NO_THROW(SimpleMappableDoseAccessor(
                doseAccessor1->getGeometricInfo(), doseAccessor2, transformDummy));

			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformDummy, interpolationLinear));
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformDummy, interpolationLinear, false));
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformDummy, interpolationLinear, true, 5.0));
			CHECK_NO_THROW(SimpleMappableDoseAccessor(doseAccessor1GeometricInfo, doseAccessor2,
			               transformDummy, interpolationNN));

			CHECK_THROW_EXPLICIT(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessorNull, transformDummy, interpolationLinear), core::NullPointerException);
			CHECK_THROW_EXPLICIT(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessor2, transformNull, interpolationLinear), core::NullPointerException);
			CHECK_THROW_EXPLICIT(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessorNull, transformNull, interpolationLinear), core::NullPointerException);
			CHECK_THROW_EXPLICIT(SimpleMappableDoseAccessor(doseAccessor1GeometricInfo, doseAccessor2,
			                     transformDummy, interpolationNull), core::NullPointerException);

			//2) test getGeometricInfo(), getGridSize(), getDoseUID() function

			CHECK_EQUAL(aSimpleMappableDoseAccessorDefault->getGeometricInfo(),
			            doseAccessor1->getGeometricInfo());
			CHECK_EQUAL(aSimpleMappableDoseAccessorDefault->getGridSize(),
			            doseAccessor1->getGeometricInfo().getNumberOfVoxels());
			CHECK_EQUAL(aSimpleMappableDoseAccessorDefault->getUID(), doseAccessor2->getUID());

			//3) test getDoseAt()

			//test valid voxels
			std::vector<VoxelGridIndex3D> voxelsAsIndexToTest;
			std::vector<DoseTypeGy> expectedValues;

			voxelsAsIndexToTest.push_back(VoxelGridIndex3D(5, 9, 8));
			voxelsAsIndexToTest.push_back(VoxelGridIndex3D(22, 15, 10));
			voxelsAsIndexToTest.push_back(VoxelGridIndex3D(30, 20, 7));

			expectedValues.push_back(5.0 * doseGridScaling);
			expectedValues.push_back(22.0 * doseGridScaling);
			expectedValues.push_back(30.0 * doseGridScaling);

			for (int i = 0; i < voxelsAsIndexToTest.size(); i++)
			{
				VoxelGridID currentId;
				doseAccessor1GeometricInfo.convert(voxelsAsIndexToTest.at(i), currentId);
				//test if the expected interpolation values are computed
				CHECK_EQUAL(aSimpleMappableDoseAccessorDefault->getValueAt(currentId),
				            expectedValues.at(i));
				//test if getDoseAt(VoxelGridIndex3D) and getDoseAt(VoxelGridD) lead to the same results
				CHECK_EQUAL(aSimpleMappableDoseAccessorDefault->getValueAt(currentId),
				            aSimpleMappableDoseAccessorDefault->getValueAt(voxelsAsIndexToTest.at(i)));
			}

			//test invalid voxels
			VoxelGridID invalidID(doseAccessor1GeometricInfo.getNumberOfVoxels() + 1);
			VoxelGridIndex3D invalidIndex(doseAccessor1GeometricInfo.getNumColumns() + 1,
			                              doseAccessor1GeometricInfo.getNumRows() + 1, doseAccessor1GeometricInfo.getNumSlices() + 1);
			CHECK_EQUAL(aSimpleMappableDoseAccessorDefault->getValueAt(invalidID), 0.0);
			CHECK_EQUAL(aSimpleMappableDoseAccessorDefault->getValueAt(invalidIndex), 0.0);
			CHECK_THROW_EXPLICIT(aSimpleMappableDoseAccessorNoPadding->getValueAt(invalidID),
			                     core::MappingOutsideOfImageException);
			CHECK_THROW_EXPLICIT(aSimpleMappableDoseAccessorNoPadding->getValueAt(invalidIndex),
			                     core::MappingOutsideOfImageException);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
