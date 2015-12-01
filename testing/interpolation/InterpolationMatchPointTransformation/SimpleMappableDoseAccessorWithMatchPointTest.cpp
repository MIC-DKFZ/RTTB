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
#include "rttbGeometricInfo.h"
#include "rttbTransformationInterface.h"
#include "rttbMatchPointTransformation.h"

#include "rttbNullPointerException.h"

#include "registrationTest.h"
#include "simpleRegistrationWorkflow.h"

namespace rttb
{
	namespace testing
	{
		static const unsigned int TargetDimension3D = 3;
		static const unsigned int MovingDimension3D = 3;
		typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
		typedef rttb::interpolation::SimpleMappableDoseAccessor SimpleMappableDoseAccessor;
		typedef map::core::RegistrationTest<MovingDimension3D, TargetDimension3D> Registration3D3DTypeTest;
		typedef Registration3D3DTypeTest::Pointer Registration3D3DTypeTestPointer;
		typedef map::core::Registration<MovingDimension3D, TargetDimension3D> Registration3D3DType;
		typedef Registration3D3DType::Pointer Registration3D3DPointer;
		typedef rttb::interpolation::NearestNeighborInterpolation NearestNeighborInterpolation;
		typedef rttb::interpolation::LinearInterpolation LinearInterpolation;
		typedef rttb::interpolation::TransformationInterface TransformationInterface;
		typedef rttb::interpolation::MatchPointTransformation MatchPointTransformation;

		/*! @brief SimpleMappableDoseAccessorWithMatchPointTest - test the API of SimpleMappableDoseAccessor with MatchPoint transform
			1) Test constructor
			2) test getDoseAt()
				a) with Identity transform
				b) with translation transform
			[3) test with rigid registration optional (if filenames are given as argument)]
		*/

		int SimpleMappableDoseAccessorWithMatchPointTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string RTDOSE_FILENAME_CONSTANT_TWO;
			std::string RTDOSE_FILENAME_INCREASE_X;
			std::string RTDOSE_FILENAME_REALISTIC = "";
			std::string CT_PLANNING = "";
			std::string CT_FRACTION = "";

			if (argc > 2)
			{
				RTDOSE_FILENAME_CONSTANT_TWO = argv[1];
				RTDOSE_FILENAME_INCREASE_X = argv[2];
			}
			else
			{
				std::cout << "at least two parameters for SimpleMappableDoseAccessorWithMatchPointTest are expected"
				          << std::endl;
				return -1;
			}

			if (argc > 5)
			{
				RTDOSE_FILENAME_REALISTIC = argv[3];
				CT_PLANNING = argv[4];
				CT_FRACTION = argv[5];
			}

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(
			    RTDOSE_FILENAME_CONSTANT_TWO.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			DoseAccessorPointer doseAccessorNull;

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(
			    RTDOSE_FILENAME_INCREASE_X.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			core::GeometricInfo doseAccessor1GeometricInfo = doseAccessor1->getGeometricInfo();

			Registration3D3DTypeTestPointer registration = Registration3D3DTypeTest::New();
			double translation[] = {0.0, 0.0, 0.0};
			registration->_translation = translation;
			registration->_limitedTarget = false;

			NearestNeighborInterpolation::Pointer interpolationNN =
			    NearestNeighborInterpolation::Pointer(new NearestNeighborInterpolation());
			LinearInterpolation::Pointer interpolationLinear = LinearInterpolation::Pointer
			        (new LinearInterpolation());
			NearestNeighborInterpolation::Pointer interpolationNull;

			TransformationInterface::Pointer transformMP = TransformationInterface::Pointer(
			            new MatchPointTransformation(
			                registration.GetPointer()));

			SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorMPIdentityLinear =
			    SimpleMappableDoseAccessor::Pointer(
			        new SimpleMappableDoseAccessor(
			            doseAccessor1GeometricInfo, doseAccessor2, transformMP, interpolationLinear));
			SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorMPIdentityNN =
			    SimpleMappableDoseAccessor::Pointer(
			        new SimpleMappableDoseAccessor(
			            doseAccessor1GeometricInfo, doseAccessor2, transformMP, interpolationNN));

			//1) Test constructor
			CHECK_NO_THROW(SimpleMappableDoseAccessor(
			                   doseAccessor1GeometricInfo, doseAccessor2, transformMP, interpolationLinear));
			CHECK_NO_THROW(SimpleMappableDoseAccessor(
			                   doseAccessor1GeometricInfo, doseAccessor2, transformMP, interpolationNN));
			CHECK_NO_THROW(MatchPointTransformation(registration.GetPointer()));
			CHECK_THROW_EXPLICIT(MatchPointTransformation(NULL), core::NullPointerException);

			//2) test getDoseAt()
			//	a) with Identity transform

			double vectorDoseAccessorStartEnd = 0.0;

			while (vectorDoseAccessorStartEnd <= 1.0)
			{
				VoxelGridID runningID = (VoxelGridID)(vectorDoseAccessorStartEnd *
				                                      (double)aSimpleMappableDoseAccessorMPIdentityLinear->getGridSize());

				CHECK_EQUAL(aSimpleMappableDoseAccessorMPIdentityLinear->getValueAt(runningID),
				            doseAccessor2->getValueAt(runningID));
				CHECK_EQUAL(aSimpleMappableDoseAccessorMPIdentityNN->getValueAt(runningID),
				            doseAccessor2->getValueAt(runningID));
				vectorDoseAccessorStartEnd += 0.1;
			}


			//	b) with translation transform


			//Second: Translation (5mm/5mm/5mm) --> in voxel: (1/1/1) as pixelspacing = 5 mm
			translation[0] = translation[1] = translation[2] = 5.0;
			registration->_translation = translation;
			SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorMPTranslationLinear =
			    SimpleMappableDoseAccessor::Pointer(new SimpleMappableDoseAccessor(
			            doseAccessor1GeometricInfo, doseAccessor2, transformMP, interpolationLinear));
			SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorMPTranslationNN =
			    SimpleMappableDoseAccessor::Pointer(
			        new SimpleMappableDoseAccessor(
			            doseAccessor1GeometricInfo, doseAccessor2, transformMP, interpolationNN));

			rttb::VoxelGridIndex3D aIndexBeforeTransformation(0, 0, 0);
			rttb::VoxelGridIndex3D aIndexAfterTransformation(1, 1, 1);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPTranslationLinear->getValueAt(aIndexBeforeTransformation),
			            doseAccessor2->getValueAt(aIndexAfterTransformation));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPTranslationNN->getValueAt(aIndexBeforeTransformation),
			            doseAccessor2->getValueAt(aIndexAfterTransformation));

			rttb::VoxelGridIndex3D aIndexBeforeTransformation2(20, 10, 10);
			rttb::VoxelGridIndex3D aIndexAfterTransformation2(21, 11, 11);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPTranslationLinear->getValueAt(aIndexBeforeTransformation2),
			            doseAccessor2->getValueAt(aIndexAfterTransformation2));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPTranslationNN->getValueAt(aIndexBeforeTransformation2),
			            doseAccessor2->getValueAt(aIndexAfterTransformation2));

			rttb::VoxelGridIndex3D aIndexBeforeTransformation3(
			    aSimpleMappableDoseAccessorMPTranslationLinear->getGeometricInfo().getNumColumns() - 2,
			    aSimpleMappableDoseAccessorMPTranslationLinear->getGeometricInfo().getNumRows() - 2,
			    aSimpleMappableDoseAccessorMPTranslationLinear->getGeometricInfo().getNumSlices() - 2);
			rttb::VoxelGridIndex3D aIndexAfterTransformation3(
			    aSimpleMappableDoseAccessorMPTranslationLinear->getGeometricInfo().getNumColumns() - 1,
			    aSimpleMappableDoseAccessorMPTranslationLinear->getGeometricInfo().getNumRows() - 1,
			    aSimpleMappableDoseAccessorMPTranslationLinear->getGeometricInfo().getNumSlices() - 1);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPTranslationLinear->getValueAt(aIndexBeforeTransformation3),
			            doseAccessor2->getValueAt(aIndexAfterTransformation3));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPTranslationNN->getValueAt(aIndexBeforeTransformation3),
			            doseAccessor2->getValueAt(aIndexAfterTransformation3));

			if (RTDOSE_FILENAME_REALISTIC != "" && CT_FRACTION != "" && CT_PLANNING != "")
			{
				//3) test with rigid registration
				//realistic background: registration from BP-CT to fraction CT, apply on planning dose that is based on BP-CT (proof of concept)

				//Target image: fraction CT, Moving image: planning CT
				simpleRegistrationWorkflow prepareRegistrationRealisticScenario(CT_FRACTION, CT_PLANNING, true);
				Registration3D3DPointer registrationRealisticScenario =
				    prepareRegistrationRealisticScenario.getRegistration();

				TransformationInterface::Pointer transformRealistic = TransformationInterface::Pointer(
				            new MatchPointTransformation(
				                registrationRealisticScenario));

				io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator3(RTDOSE_FILENAME_REALISTIC.c_str());
				DoseAccessorPointer doseAccessor3(doseAccessorGenerator3.generateDoseAccessor());

				core::GeometricInfo geoInfoRealistic;
				geoInfoRealistic.setNumColumns(
				    prepareRegistrationRealisticScenario.getTargetImage()->GetLargestPossibleRegion().GetSize()[0]);
				geoInfoRealistic.setNumRows(
				    prepareRegistrationRealisticScenario.getTargetImage()->GetLargestPossibleRegion().GetSize()[1]);
				geoInfoRealistic.setNumSlices(
				    prepareRegistrationRealisticScenario.getTargetImage()->GetLargestPossibleRegion().GetSize()[2]);

				//Dose is related to BP-CT, map dose to fraction CT geometry
				SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorRealisticScenarioLinear =
				    SimpleMappableDoseAccessor::Pointer(new SimpleMappableDoseAccessor(geoInfoRealistic,
				                                        doseAccessor3, transformRealistic, interpolationLinear));

				//combination of 0, size/2 and size to check as coordinates
				std::vector<unsigned int> coordinatesToCheckX, coordinatesToCheckY, coordinatesToCheckZ;
				coordinatesToCheckX.push_back(0);
				coordinatesToCheckX.push_back(geoInfoRealistic.getNumColumns() / 2);
				coordinatesToCheckX.push_back(geoInfoRealistic.getNumColumns() - 1);
				coordinatesToCheckY.push_back(0);
				coordinatesToCheckY.push_back(geoInfoRealistic.getNumRows() / 2);
				coordinatesToCheckY.push_back(geoInfoRealistic.getNumRows() - 1);
				coordinatesToCheckZ.push_back(0);
				coordinatesToCheckZ.push_back(geoInfoRealistic.getNumSlices() / 2);
				coordinatesToCheckZ.push_back(geoInfoRealistic.getNumSlices() - 1);

				//Pixels are inside the fraction CT image and mapping should work (even if they map outside of doseAccessor3)
				for (unsigned int i = 0; i < coordinatesToCheckX.size(); ++i)
				{
					for (unsigned int j = 0; j < coordinatesToCheckY.size(); ++j)
					{
						for (unsigned int k = 0; k < coordinatesToCheckZ.size(); ++k)
						{
							CHECK_NO_THROW(aSimpleMappableDoseAccessorRealisticScenarioLinear->getValueAt(VoxelGridIndex3D(
							                   coordinatesToCheckX.at(i), coordinatesToCheckY.at(j), coordinatesToCheckZ.at(k))));
						}
					}
				}

			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
