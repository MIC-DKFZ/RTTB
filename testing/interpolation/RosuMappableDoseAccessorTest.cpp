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

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbMatchPointTransformation.h"
#include "rttbRosuMappableDoseAccessor.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbLinearInterpolation.h"
#include "rttbITKImageDoseAccessor.h"

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
		typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
		typedef rttb::interpolation::RosuMappableDoseAccessor RosuMappableDoseAccessor;
		typedef map::core::RegistrationTest<MovingDimension3D, TargetDimension3D> Registration3D3DTypeTest;
		typedef Registration3D3DTypeTest::Pointer Registration3D3DTypeTestPointer;
		typedef map::core::Registration<MovingDimension3D, TargetDimension3D> Registration3D3DType;
		typedef Registration3D3DType::Pointer Registration3D3DPointer;
		typedef rttb::interpolation::TransformationInterface TransformationInterface;
		typedef rttb::interpolation::MatchPointTransformation MatchPointTransformation;
		typedef RosuMappableDoseAccessor::Pointer RosuMappableDoseAccessorPointer;
		typedef rttb::interpolation::MappableDoseAccessorBase MappableDoseAccessorBase;

		/*! @brief RosuMappableDoseAccessorTest - test the API of RosuMappableDoseAccessor
			1) test constructor
			2) test with registration stub
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
				std::cout << "at least two parameters for MappableDoseAccessorTest are expected" << std::endl;
				return -1;
			}

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(
			    RTDOSE_FILENAME_CONSTANT_TWO.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			DoseAccessorPointer doseAccessorNull;

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(
			    RTDOSE_FILENAME_INCREASE_X.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			const double doseGridScalingDose2 = 2.822386e-005;

			Registration3D3DTypeTestPointer registration = Registration3D3DTypeTest::New();
			double translation[] = {0.0, 0.0, 0.0};
			registration->translation = translation;
			registration->_limitedTarget = false;

			TransformationInterface::Pointer transformMP = TransformationInterface::Pointer(
			            new MatchPointTransformation(registration));
			boost::shared_ptr<MatchPointTransformation> transformNull;

			MappableDoseAccessorBase::Pointer aMappableDoseAccessorDefault =
			    MappableDoseAccessorBase::Pointer(new RosuMappableDoseAccessor(
			            doseAccessor1->getGeometricInfo(), doseAccessor2, transformMP));

			//1) test constructors
			CHECK_NO_THROW(RosuMappableDoseAccessor test(doseAccessor1->getGeometricInfo(), doseAccessor2,
			               transformMP));


			//2) test with registration stub
			//First: Identity Transformation (nothing changes). Should lead to the same value although the subvoxel values are different (but symmetric with respect to the voxel center)

			//outside: should have default value 0
			CHECK_EQUAL(aMappableDoseAccessorDefault->getDoseAt(
			                aMappableDoseAccessorDefault->getGridSize() + 1), 0.0);

			const core::GeometricInfo geoInfoTarget = doseAccessor1->getGeometricInfo();

			const VoxelGridIndex3D indexValid(geoInfoTarget.getNumColumns() - 1, geoInfoTarget.getNumRows() - 1,
			                                  geoInfoTarget.getNumSlices() - 1);

			const VoxelGridID idValid(indexValid[2] * geoInfoTarget.getNumColumns() * geoInfoTarget.getNumRows()
			                          + indexValid[1] * geoInfoTarget.getNumColumns()
			                          + indexValid[0]);

			//check if getDoseAt(VoxelGridID) and getDoseAt(VoxelGridIndex3D) lead to same result
			CHECK_NO_THROW(aMappableDoseAccessorDefault->getDoseAt(indexValid));
			CHECK_EQUAL(aMappableDoseAccessorDefault->getDoseAt(indexValid),
			            aMappableDoseAccessorDefault->getDoseAt(idValid));
			CHECK_CLOSE(aMappableDoseAccessorDefault->getDoseAt(indexValid),
			            doseGridScalingDose2 * indexValid.x(),
			            errorConstant);
			//dose value equals x-position. 0.5*(0.25+0.75)=0.5 (compute interpolation value at x-pos 0.25 and 0.75)
			CHECK_CLOSE(aMappableDoseAccessorDefault->getDoseAt(VoxelGridID(0)), 0.5 * doseGridScalingDose2,
			            errorConstant);

			//Second: Translation (2.5mm/2.5mm/2.5mm) --> in voxel: (0.5/0.5/0.5) as pixelspacing = 5 mm
			translation[0] = translation[1] = translation[2] = 2.5;
			registration->translation = translation;
			rttb::VoxelGridIndex3D aIndex(23, 11, 9);

			//value equals x-position. 0.5*((indexValid.x()+0.25+translation_x)+(indexValid.x()+0.75+translation_x))=1
			CHECK_CLOSE(aMappableDoseAccessorDefault->getDoseAt(aIndex),
			            (aIndex.x() + 1.0)*doseGridScalingDose2, errorConstant);

			rttb::VoxelGridIndex3D aIndex2(49, 32, 29);

			CHECK_CLOSE(aMappableDoseAccessorDefault->getDoseAt(aIndex2),
			            (aIndex2.x() + 1.0)*doseGridScalingDose2, errorConstant);

			//@todo: more tests needed?

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
