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

#include "rttbLQModelAccessor.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace testing
	{

		/*! @brief LQModelAccessorTest.
		1) Test constructor
		2) Test getGeometricInfo()
		3) Test getValueAt()

		*/
		int LQModelAccessorTest(int argc, char* argv[])
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

				std::cout << "at least two parameters for LQModelAccessorTest are expected" <<
				          std::endl;
				return -1;
			}

			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(
			    RTDOSE_FILENAME_CONSTANT_TWO.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(
			    RTDOSE_FILENAME_INCREASE_X.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			core::GeometricInfo doseAccessor1GeometricInfo = doseAccessor1->getGeometricInfo();
			core::GeometricInfo doseAccessor2GeometricInfo = doseAccessor2->getGeometricInfo();

			DoseAccessorPointer doseAccessorNull;

			core::AccessorInterface::AccessorPointer LQWithConstantDose;
			core::AccessorInterface::AccessorPointer LQWithConstantDoseDoseScalingTwo;
			core::AccessorInterface::AccessorPointer LQWithIncreaseXDose;

			//1) test constructor
			CHECK_THROW_EXPLICIT(models::LQModelAccessor(doseAccessorNull, 0, 0), core::InvalidDoseException);
			CHECK_THROW_EXPLICIT(models::LQModelAccessor(doseAccessor1, 0.2, 0.02, -1), core::InvalidParameterException);

			CHECK_NO_THROW(LQWithConstantDose = boost::make_shared<models::LQModelAccessor>(doseAccessor1,
			                                    0.2, 0.02));
			CHECK_NO_THROW(LQWithConstantDoseDoseScalingTwo = boost::make_shared<models::LQModelAccessor>(doseAccessor1,
			               0.2, 0.02, 2.0));
			CHECK_NO_THROW(LQWithIncreaseXDose = boost::make_shared<models::LQModelAccessor>(doseAccessor2,
			                                     0.3, 0.01));

			//2) Test getGeometricInfo()
			CHECK_EQUAL(LQWithConstantDose->getGeometricInfo(), doseAccessor1GeometricInfo);
			CHECK_EQUAL(LQWithIncreaseXDose->getGeometricInfo(), doseAccessor2GeometricInfo);

			//3) Test getValueAt()

			models::BioModelParamType expectedLQWithDoseTwo = exp(-(0.2 * 2 + (0.02 * 2 * 2)));
			CHECK_EQUAL(LQWithConstantDose->getValueAt(0), expectedLQWithDoseTwo);
			CHECK_EQUAL(LQWithConstantDose->getValueAt(LQWithConstantDose->getGridSize() - 1), expectedLQWithDoseTwo);
			CHECK_EQUAL(LQWithConstantDose->getValueAt(VoxelGridIndex3D(1, 2, 6)), expectedLQWithDoseTwo);
			CHECK_EQUAL(LQWithConstantDose->getValueAt(VoxelGridIndex3D(65, 40, 60)), expectedLQWithDoseTwo);

			models::BioModelParamType expectedLQWithDoseFour = exp(-(0.2 * 4 + (0.02 * 4 * 4)));
			CHECK_EQUAL(LQWithConstantDoseDoseScalingTwo->getValueAt(0), expectedLQWithDoseFour);
			CHECK_EQUAL(LQWithConstantDoseDoseScalingTwo->getValueAt(LQWithConstantDose->getGridSize() - 1),
			            expectedLQWithDoseFour);
			CHECK_EQUAL(LQWithConstantDoseDoseScalingTwo->getValueAt(VoxelGridIndex3D(1, 2, 6)),
			            expectedLQWithDoseFour);
			CHECK_EQUAL(LQWithConstantDoseDoseScalingTwo->getValueAt(VoxelGridIndex3D(65, 40, 60)),
			            expectedLQWithDoseFour);


			models::BioModelParamType expectedLQWithIncreaseX = exp(-(0.3 * 66 * 2.822386e-5 + (0.01 * 66 * 2.822386e-5 * 66 *
			        2.822386e-5)));
			CHECK_EQUAL(LQWithIncreaseXDose->getValueAt(0), 1);
			CHECK_CLOSE(LQWithIncreaseXDose->getValueAt(LQWithIncreaseXDose->getGridSize() - 1), expectedLQWithIncreaseX,
			            errorConstant);
			expectedLQWithIncreaseX = exp(-(0.3 * 1 * 2.822386e-5 + (0.01 * 1 * 2.822386e-5 * 1 * 2.822386e-5)));
			CHECK_CLOSE(LQWithIncreaseXDose->getValueAt(VoxelGridIndex3D(1, 2, 6)), expectedLQWithIncreaseX, errorConstant);
			expectedLQWithIncreaseX = exp(-(0.3 * 45 * 2.822386e-5 + (0.01 * 45 * 2.822386e-5 * 45 * 2.822386e-5)));
			CHECK_CLOSE(LQWithIncreaseXDose->getValueAt(VoxelGridIndex3D(45, 40, 60)), expectedLQWithIncreaseX,
			            errorConstant);

			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb