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
// @version $Revision: 741 $ (last changed revision)
// @date    $Date: 2014-09-16 16:34:22 +0200 (Di, 16 Sep 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbLinearInterpolation.h"
#include "rttbTransformationInterface.h"
#include "rttbITKTransformation.h"

#include "rttbNullPointerException.h"

#include "itkTranslationTransform.h"

namespace rttb
{
	namespace testing
	{
		typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
		typedef rttb::interpolation::SimpleMappableDoseAccessor SimpleMappableDoseAccessor;
		typedef rttb::interpolation::NearestNeighborInterpolation NearestNeighborInterpolation;
		typedef rttb::interpolation::LinearInterpolation LinearInterpolation;
		typedef rttb::interpolation::TransformationInterface TransformationInterface;
		typedef rttb::interpolation::ITKTransformation ITKTransformation;
		typedef itk::TranslationTransform<DoseTypeGy, 3> TranslationTransformType;

		/*! @brief SimpleMappableDoseAccessorWithITKTest - test the API of SimpleMappableDoseAccessor with ITK transformation
			1) Test constructor
			2) test getDoseAt()
				a) with Identity transform
				b) with translation transform
		*/

		int SimpleMappableDoseAccessorWithITKTest(int argc, char* argv[])
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
				std::cout << "at least two parameters for SimpleMappableDoseAccessorWithITKTest are expected" <<
				          std::endl;
				return -1;
			}

			const double doseGridScaling = 2.822386e-005;

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(
			    RTDOSE_FILENAME_CONSTANT_TWO.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(
			    RTDOSE_FILENAME_INCREASE_X.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			core::GeometricInfo doseAccessor1GeometricInfo = doseAccessor1->getGeometricInfo();

			NearestNeighborInterpolation::Pointer interpolationNN =
			    NearestNeighborInterpolation::Pointer(new NearestNeighborInterpolation());
			LinearInterpolation::Pointer interpolationLinear = LinearInterpolation::Pointer
			        (new LinearInterpolation());

			//auto transformITKIdentity = TransformationInterface::Pointer(new ITKTransformation(
			//                                IdentityTransformType::New()));

			TranslationTransformType::Pointer transformITKIdentityTemporary =
			    TranslationTransformType::New();
			TranslationTransformType::OutputVectorType translationIdentity;
			translationIdentity[0] = 0.0;
			translationIdentity[1] = 0.0;
			translationIdentity[2] = 0.0;
			transformITKIdentityTemporary->Translate(translationIdentity);

			TransformationInterface::Pointer transformITKIdentity = TransformationInterface::Pointer(new ITKTransformation(
			            transformITKIdentityTemporary));

			TranslationTransformType::Pointer transformITKTranslationTemporary =
			    TranslationTransformType::New();
			TranslationTransformType::OutputVectorType translation;
			translation[0] = 5.0;
			translation[1] = 5.0;
			translation[2] = 5.0;
			transformITKTranslationTemporary->Translate(translation);

			TransformationInterface::Pointer transformITKTranslation = TransformationInterface::Pointer(new ITKTransformation(
			            transformITKTranslationTemporary));

			SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorITKIdentity = SimpleMappableDoseAccessor::Pointer(
			            new SimpleMappableDoseAccessor(
			                doseAccessor1GeometricInfo, doseAccessor2, transformITKIdentity, interpolationLinear));
			SimpleMappableDoseAccessor::Pointer aSimpleMappableDoseAccessorITKTranslation = SimpleMappableDoseAccessor::Pointer(
			            new SimpleMappableDoseAccessor(
			                doseAccessor1GeometricInfo, doseAccessor2, transformITKTranslation, interpolationLinear));

			//1) Test constructor
			CHECK_NO_THROW(SimpleMappableDoseAccessor(
			                   doseAccessor1GeometricInfo, doseAccessor2, transformITKIdentity, interpolationLinear));
			CHECK_NO_THROW(SimpleMappableDoseAccessor(
			                   doseAccessor1GeometricInfo, doseAccessor2, transformITKTranslation, interpolationLinear));
			CHECK_NO_THROW(ITKTransformation(transformITKTranslationTemporary.GetPointer()));
			CHECK_THROW_EXPLICIT(ITKTransformation(NULL), core::NullPointerException);

			//2) test getDoseAt()
			//	a) with Identity transform

			//test valid voxels
			std::vector<VoxelGridIndex3D> voxelsAsIndexToTest;
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
				doseAccessor1GeometricInfo.convert(voxelsAsIndexToTest.at(i), currentId);
				//test if the expected interpolation values are computed
				CHECK_EQUAL(aSimpleMappableDoseAccessorITKIdentity->getValueAt(voxelsAsIndexToTest.at(i)),
				            expectedValues.at(i));
				//test if getDoseAt(VoxelGridIndex3D) and getDoseAt(VoxelGridD) lead to the same results
				CHECK_EQUAL(aSimpleMappableDoseAccessorITKIdentity->getValueAt(voxelsAsIndexToTest.at(i)),
				            aSimpleMappableDoseAccessorITKIdentity->getValueAt(currentId));
			}


			//no tests with invalid IDs, this has been done already tested in SimpleMappableDoseAccessorTest

			//b) with translation transform

			//translation of one voxel in each direction
			expectedValues.clear();
			expectedValues.push_back(6.0 * doseGridScaling);
			expectedValues.push_back(23.0 * doseGridScaling);
			expectedValues.push_back(31.0 * doseGridScaling);

			for (int i = 0; i < voxelsAsIndexToTest.size(); i++)
			{
				VoxelGridID currentId;
				doseAccessor1GeometricInfo.convert(voxelsAsIndexToTest.at(i), currentId);
				//test if the expected interpolation values are computed
				CHECK_EQUAL(aSimpleMappableDoseAccessorITKTranslation->getValueAt(voxelsAsIndexToTest.at(i)),
				            expectedValues.at(i));
				//test if getDoseAt(VoxelGridIndex3D) and getDoseAt(VoxelGridD) lead to the same results
				CHECK_EQUAL(aSimpleMappableDoseAccessorITKTranslation->getValueAt(voxelsAsIndexToTest.at(i)),
				            aSimpleMappableDoseAccessorITKTranslation->getValueAt(currentId));
			}


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
