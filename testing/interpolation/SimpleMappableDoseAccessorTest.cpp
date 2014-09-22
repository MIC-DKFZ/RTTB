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

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxDoseAccessor.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbLinearInterpolation.h"
#include "rttbITKImageDoseAccessor.h"
#include "rttbTransformationInterface.h"
#include "rttbMatchPointTransformation.h"

#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"

#include "itkImage.h"

#include "registrationTest.h"
#include "registrationAlgorithm.h"

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
		typedef rttb::interpolation::MappableDoseAccessorBase MappableDoseAccessorBase;

		/*! @brief SimpleMappableDoseAccessorTest - test the API of SimpleMappableDoseAccessor + MappableDoseAccessorBase
			1) test constructor
			2) test getGeometricInfo(), validId(), validIndex() function
			3) test with registration stub
			optional (depends on input parameters):
			4) test with rigid registration
		*/

		int SimpleMappableDoseAccessorTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string RTDOSE_FILENAME_CONSTANT_TWO;
			std::string RTDOSE_FILENAME_INCREASE_X;
			std::string RTDOSE_FILENAME_REALISTIC;
			std::string CT_PLANNING;
			std::string CT_FRACTION;

			if (argc > 2)
			{
				RTDOSE_FILENAME_CONSTANT_TWO = argv[1];
				RTDOSE_FILENAME_INCREASE_X = argv[2];
				RTDOSE_FILENAME_REALISTIC = "";
				CT_PLANNING = "";
				CT_FRACTION = "";
			}
			else
			{
				std::cout << "at least two parameters for MappableDoseAccessorTest are expected" << std::endl;
				return -1;
			}

			if (argc > 5)
			{
				RTDOSE_FILENAME_CONSTANT_TWO = argv[1];
				RTDOSE_FILENAME_INCREASE_X = argv[2];
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

			Registration3D3DTypeTestPointer registration = Registration3D3DTypeTest::New();
			double translation[] = {0.0, 0.0, 0.0};
			registration->translation = translation;
			registration->_limitedTarget = false;

			auto interpolationNN =
			    NearestNeighborInterpolation::Pointer(new NearestNeighborInterpolation());
			auto interpolationLinear = boost::shared_ptr<LinearInterpolation>
			                           (new LinearInterpolation());
			boost::shared_ptr<NearestNeighborInterpolation> interpolationNull;

			auto transformMP = TransformationInterface::Pointer(new MatchPointTransformation(registration));
			boost::shared_ptr<MatchPointTransformation> transformNull;

			auto aSimpleMappableDoseAccessorMPDefaultNN = MappableDoseAccessorBase::Pointer(
			            new SimpleMappableDoseAccessor(doseAccessor1->getGeometricInfo(), doseAccessor2, transformMP,
			                    interpolationNN));
			auto aSimpleMappableDoseAccessorMPNoPaddingNN = MappableDoseAccessorBase::Pointer(
			            new SimpleMappableDoseAccessor(doseAccessor1->getGeometricInfo(), doseAccessor2, transformMP,
			                    interpolationNN, false));
			auto aSimpleMappableDoseAccessorMPPaddingNN = MappableDoseAccessorBase::Pointer(
			            new SimpleMappableDoseAccessor(doseAccessor1->getGeometricInfo(), doseAccessor2, transformMP,
			                    interpolationNN, true, 42.0));
			auto aSimpleMappableDoseAccessorMPDefaultLinear = MappableDoseAccessorBase::Pointer(
			            new SimpleMappableDoseAccessor(doseAccessor1->getGeometricInfo(), doseAccessor2, transformMP,
			                    interpolationLinear));
			auto aSimpleMappableDoseAccessorMPNoPaddingLinear = MappableDoseAccessorBase::Pointer(
			            new SimpleMappableDoseAccessor(doseAccessor1->getGeometricInfo(), doseAccessor2, transformMP,
			                    interpolationLinear, false));
			auto aSimpleMappableDoseAccessorMPPaddingLinear = MappableDoseAccessorBase::Pointer(
			            new SimpleMappableDoseAccessor(doseAccessor1->getGeometricInfo(), doseAccessor2, transformMP,
			                    interpolationLinear, true, 42.0));

			const core::GeometricInfo geoInfoMoving = doseAccessor1->getGeometricInfo();
			core::GeometricInfo geoInfoTarget = geoInfoMoving;
			geoInfoTarget.setNumColumns(geoInfoTarget.getNumColumns() / 2);
			geoInfoTarget.setNumRows(geoInfoTarget.getNumRows() / 2);
			geoInfoTarget.setNumSlices(geoInfoTarget.getNumSlices() / 2);
			geoInfoTarget.setPixelSpacingRow(geoInfoTarget.getPixelSpacingRow() * 2.0);
			geoInfoTarget.setPixelSpacingColumn(geoInfoTarget.getPixelSpacingColumn() * 2.0);
			geoInfoTarget.setSliceThickness(geoInfoTarget.getSliceThickness() * 2.0);

			const VoxelGridIndex3D indexValid(geoInfoTarget.getNumColumns() - 1, geoInfoTarget.getNumRows() - 1,
			                                  geoInfoTarget.getNumSlices() - 1);
			const VoxelGridIndex3D indexInvalid(geoInfoMoving.getNumColumns() - 1,
			                                    geoInfoMoving.getNumRows() - 1, geoInfoMoving.getNumSlices() - 1);

			const VoxelGridID idValid(indexValid[2] * geoInfoTarget.getNumColumns() * geoInfoTarget.getNumRows()
			                          +
			                          indexValid[1] * geoInfoTarget.getNumColumns()
			                          + indexValid[0]);
			const VoxelGridID idInvalid(indexInvalid[2] * geoInfoTarget.getNumColumns() *
			                            geoInfoTarget.getNumRows() +
			                            indexInvalid[1] * geoInfoTarget.getNumColumns()
			                            + indexInvalid[0]);

			auto aMappableDoseAccessorMPChangedGeometricInfo = MappableDoseAccessorBase::Pointer(
			            new SimpleMappableDoseAccessor(geoInfoTarget, doseAccessor1, transformMP,
			                    interpolationNN));

			//1) test constructors
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformMP, interpolationNN));
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformMP, interpolationNN, false));
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformMP, interpolationNN, true, 5.0));
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformMP, interpolationLinear));
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformMP, interpolationLinear, false));
			CHECK_NO_THROW(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformMP, interpolationLinear, true, 5.0));

			CHECK_THROW_EXPLICIT(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessorNull, transformMP, interpolationNN), core::NullPointerException);
			CHECK_THROW_EXPLICIT(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessor2, transformNull, interpolationNN), core::NullPointerException);
			CHECK_THROW_EXPLICIT(SimpleMappableDoseAccessor test(doseAccessor1->getGeometricInfo(),
			                     doseAccessor2,
			                     transformMP, interpolationNull), core::NullPointerException);

			//2) test getGeometricInfo(), validId(), validIndex() function

			CHECK_EQUAL(aMappableDoseAccessorMPChangedGeometricInfo->getGeometricInfo(), geoInfoTarget);

			CHECK_EQUAL(aMappableDoseAccessorMPChangedGeometricInfo->validID(idValid), true);
			CHECK_EQUAL(aMappableDoseAccessorMPChangedGeometricInfo->validID(idInvalid), false);

			CHECK_EQUAL(aMappableDoseAccessorMPChangedGeometricInfo->validIndex(indexValid), true);
			CHECK_EQUAL(aMappableDoseAccessorMPChangedGeometricInfo->validIndex(indexInvalid), false);


			//3) test with registration stub
			//First: Identity Transformation (nothing changes)
			double vectorDoseAccessorStartEnd = 0.0;

			while (vectorDoseAccessorStartEnd <= 1.0)
			{
				VoxelGridID id = (VoxelGridID)(vectorDoseAccessorStartEnd *
				                               (double)aSimpleMappableDoseAccessorMPDefaultNN->getGridSize());
				CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultNN->getDoseAt(id), doseAccessor2->getDoseAt(id));
				CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultLinear->getDoseAt(id),
				            doseAccessor2->getDoseAt(id));
				vectorDoseAccessorStartEnd += 0.1;
			}

			CHECK_THROW_EXPLICIT(aSimpleMappableDoseAccessorMPNoPaddingNN->getDoseAt(
			                         aSimpleMappableDoseAccessorMPNoPaddingNN->getGridSize() + 1), core::MappingOutsideOfImageException);
			CHECK_NO_THROW(aSimpleMappableDoseAccessorMPDefaultNN->getDoseAt(
			                   aSimpleMappableDoseAccessorMPDefaultNN->getGridSize() + 1));
			CHECK_NO_THROW(aSimpleMappableDoseAccessorMPPaddingNN->getDoseAt(
			                   aSimpleMappableDoseAccessorMPPaddingNN->getGridSize() + 1));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultNN->getDoseAt(
			                aSimpleMappableDoseAccessorMPDefaultNN->getGridSize() + 1), 0.0);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPPaddingNN->getDoseAt(
			                aSimpleMappableDoseAccessorMPPaddingNN->getGridSize() + 1), 42.0);

			CHECK_THROW_EXPLICIT(aSimpleMappableDoseAccessorMPNoPaddingLinear->getDoseAt(
			                         aSimpleMappableDoseAccessorMPNoPaddingLinear->getGridSize() + 1),
			                     core::MappingOutsideOfImageException);
			CHECK_NO_THROW(aSimpleMappableDoseAccessorMPDefaultLinear->getDoseAt(
			                   aSimpleMappableDoseAccessorMPDefaultLinear->getGridSize() + 1));
			CHECK_NO_THROW(aSimpleMappableDoseAccessorMPPaddingLinear->getDoseAt(
			                   aSimpleMappableDoseAccessorMPPaddingLinear->getGridSize() + 1));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultLinear->getDoseAt(
			                aSimpleMappableDoseAccessorMPDefaultLinear->getGridSize() + 1), 0.0);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPPaddingLinear->getDoseAt(
			                aSimpleMappableDoseAccessorMPPaddingLinear->getGridSize() + 1), 42.0);

			//check if getDoseAt(VoxelGridID) and getDoseAt(VoxelGridIndex3D) lead to same result
			CHECK_NO_THROW(aMappableDoseAccessorMPChangedGeometricInfo->getDoseAt(indexValid));
			CHECK_NO_THROW(aMappableDoseAccessorMPChangedGeometricInfo->getDoseAt(indexInvalid));
			CHECK_EQUAL(aMappableDoseAccessorMPChangedGeometricInfo->getDoseAt(indexValid),
			            aMappableDoseAccessorMPChangedGeometricInfo->getDoseAt(idValid));
			CHECK_EQUAL(aMappableDoseAccessorMPChangedGeometricInfo->getDoseAt(indexInvalid), 0.0);

			//Second: Translation (5mm/5mm/5mm) --> in voxel: (1/1/1) as pixelspacing = 5 mm
			translation[0] = translation[1] = translation[2] = 5.0;
			registration->translation = translation;
			rttb::VoxelGridIndex3D aIndexBeforeTransformation(0, 0, 0);
			rttb::VoxelGridIndex3D aIndexAfterTransformation(1, 1, 1);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultNN->getDoseAt(aIndexBeforeTransformation),
			            doseAccessor2->getDoseAt(aIndexAfterTransformation));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultLinear->getDoseAt(aIndexBeforeTransformation),
			            doseAccessor2->getDoseAt(aIndexAfterTransformation));

			rttb::VoxelGridIndex3D aIndexBeforeTransformation2(20, 10, 10);
			rttb::VoxelGridIndex3D aIndexAfterTransformation2(21, 11, 11);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultNN->getDoseAt(aIndexBeforeTransformation2),
			            doseAccessor2->getDoseAt(aIndexAfterTransformation2));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultLinear->getDoseAt(aIndexBeforeTransformation2),
			            doseAccessor2->getDoseAt(aIndexAfterTransformation2));

			rttb::VoxelGridIndex3D aIndexBeforeTransformation3(
			    aSimpleMappableDoseAccessorMPDefaultNN->getGeometricInfo().getNumColumns() - 2,
			    aSimpleMappableDoseAccessorMPDefaultNN->getGeometricInfo().getNumRows() - 2,
			    aSimpleMappableDoseAccessorMPDefaultNN->getGeometricInfo().getNumSlices() - 2);
			rttb::VoxelGridIndex3D aIndexAfterTransformation3(
			    aSimpleMappableDoseAccessorMPDefaultNN->getGeometricInfo().getNumColumns() - 1,
			    aSimpleMappableDoseAccessorMPDefaultNN->getGeometricInfo().getNumRows() - 1,
			    aSimpleMappableDoseAccessorMPDefaultNN->getGeometricInfo().getNumSlices() - 1);
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultNN->getDoseAt(aIndexBeforeTransformation3),
			            doseAccessor2->getDoseAt(aIndexAfterTransformation3));
			CHECK_EQUAL(aSimpleMappableDoseAccessorMPDefaultLinear->getDoseAt(aIndexBeforeTransformation3),
			            doseAccessor2->getDoseAt(aIndexAfterTransformation3));

			if (RTDOSE_FILENAME_REALISTIC != "" && CT_FRACTION != "" && CT_PLANNING != "")
			{
				//4) test with rigid registration
				//realistic background: registration from BP-CT to fraction CT, apply on planning dose that is based on BP-CT (proof of concept)

				//to prevent error if files are not in repository
				if (boost::filesystem::exists(CT_FRACTION) && boost::filesystem::exists(CT_PLANNING))
				{

					//Target image: fraction CT, Moving image: planning CT
					registrationAlgorithm prepareRegistrationRealisticScenario(CT_FRACTION, CT_PLANNING, true);
					Registration3D3DPointer registrationRealisticScenario =
					    prepareRegistrationRealisticScenario.getRegistration();

					auto transformRealistic = TransformationInterface::Pointer(new MatchPointTransformation(
					                              registrationRealisticScenario));

					io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator3(RTDOSE_FILENAME_REALISTIC.c_str());
					DoseAccessorPointer doseAccessor3(doseAccessorGenerator3.generateDoseAccessor());

					core::GeometricInfo geoInfoRealistic = io::itk::convertToGeometricInfo(
					        prepareRegistrationRealisticScenario.getTargetImage());

					//Dose is related to BP-CT, map dose to fraction CT geometry
					auto aSimpleMappableDoseAccessorRealisticScenarioLinear = MappableDoseAccessorBase::Pointer(
					            new SimpleMappableDoseAccessor(geoInfoRealistic,
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
								CHECK_NO_THROW(aSimpleMappableDoseAccessorRealisticScenarioLinear->getDoseAt(VoxelGridIndex3D(
								                   coordinatesToCheckX.at(i), coordinatesToCheckY.at(j), coordinatesToCheckZ.at(k))));
							}
						}
					}
				}
				else
				{
					std::cout << CT_PLANNING << " and " << CT_FRACTION <<
					          " files are missing. Rigid registration tests are not performed." << std::endl;
				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
