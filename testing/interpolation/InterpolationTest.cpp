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

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbLinearInterpolation.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"

namespace rttb
{
	namespace testing
	{

		typedef rttb::interpolation::NearestNeighborInterpolation NearestNeighborInterpolation;
		typedef rttb::interpolation::LinearInterpolation LinearInterpolation;
		typedef rttb::core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;

		/*! @brief InterpolationTest - tests only interpolation
				1) test both interpolation types with simple image (Dose = 2)
				2) test both interpolation types with increasing x image values image (Dose = y value)
				3) test exception handling
			*/

		int InterpolationTest(int argc, char* argv[])
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
				std::cout << "at least two parameters for InterpolationTest expected" << std::endl;
				return -1;
			}

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(
			    RTDOSE_FILENAME_CONSTANT_TWO.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			DoseAccessorPointer doseAccessorNull;

			rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(
			    RTDOSE_FILENAME_INCREASE_X.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			//doseAccessor1 is used as dose image
      auto interpolationNN = boost::make_shared<rttb::interpolation::NearestNeighborInterpolation>();
			interpolationNN->setAccessorPointer(doseAccessor1);
			auto interpolationLinear = boost::make_shared<rttb::interpolation::LinearInterpolation>();
			interpolationLinear->setAccessorPointer(doseAccessor1);

			//doseAccessor2 is used as dose image.
			//RTDOSE_FILENAME_INCREASE_X and RTDOSE_FILENAME_CONSTANT_TWO have the same GeometricInfo
			auto interpolationNN2 = boost::make_shared<rttb::interpolation::NearestNeighborInterpolation>();
			interpolationNN2->setAccessorPointer(doseAccessor2);
			auto interpolationLinear2 = boost::make_shared<rttb::interpolation::LinearInterpolation>();
			interpolationLinear2->setAccessorPointer(doseAccessor2);

			auto interpolationNullNN = boost::make_shared<rttb::interpolation::NearestNeighborInterpolation>();
			auto interpolationNullLinear = boost::make_shared<rttb::interpolation::LinearInterpolation>();

			rttb::WorldCoordinate3D imagePositionPatient =
			    doseAccessor1->getGeometricInfo().getImagePositionPatient();
			rttb::SpacingVectorType3D pixelSpacing = doseAccessor1->getGeometricInfo().getSpacing();
			unsigned int size[] = {doseAccessor1->getGeometricInfo().getNumColumns(), doseAccessor1->getGeometricInfo().getNumRows(), doseAccessor1->getGeometricInfo().getNumSlices()};

			//Which voxels to check is irrelevant. The following three situations are checked:
            // - exactly in-between two voxels: v_i=(0.5*v1 + 0.5*v2) --> target 0.5
            // - the middle of a voxel: v_i = 1*v1 --> target 0 (semantically equivalent with target 1 and left/right flipped)
            // - 10% shifted from the middle: v_i=0.1*v1 + 0.9*v2 --> target 0.9
			std::vector<double> coordinatesX;
			coordinatesX.push_back(imagePositionPatient.x() - (pixelSpacing.x() * 0.5));
            coordinatesX.push_back(imagePositionPatient.x() + 5 * pixelSpacing.x());
            coordinatesX.push_back(imagePositionPatient.x() + 9.9 * pixelSpacing.x());
			std::vector<double> coordinatesY;
			coordinatesY.push_back(imagePositionPatient.y() - (pixelSpacing.y() * 0.5));
            coordinatesY.push_back(imagePositionPatient.y() + 5 * pixelSpacing.y());
            coordinatesY.push_back(imagePositionPatient.x() + 9.9 * pixelSpacing.y());
			std::vector<double> coordinatesZ;
			coordinatesZ.push_back(imagePositionPatient.z() - (pixelSpacing.z() * 0.5));
            coordinatesZ.push_back(imagePositionPatient.z() + 5 * pixelSpacing.z());
            coordinatesZ.push_back(imagePositionPatient.z() + 9.9 * pixelSpacing.z());

			std::vector<rttb::WorldCoordinate3D> coordinatesToCheck;

			for (unsigned int x = 0; x < coordinatesX.size(); x++)
			{
				for (unsigned int y = 0; y < coordinatesY.size(); y++)
				{
					for (unsigned int z = 0; z < coordinatesZ.size(); z++)
					{
						coordinatesToCheck.push_back(rttb::WorldCoordinate3D(coordinatesX.at(x),
						                             coordinatesY.at(y), coordinatesZ.at(z)));
					}
				}
			}

			rttb::WorldCoordinate3D positionOutsideOfImageLeft = imagePositionPatient - rttb::WorldCoordinate3D(
			            pixelSpacing.x() * 2.0, pixelSpacing.y() * 2.0, pixelSpacing.z() * 2.0);
            rttb::WorldCoordinate3D positionOutsideOfImageRight = imagePositionPatient +
                rttb::WorldCoordinate3D(size[0] * pixelSpacing.x(), size[1] * pixelSpacing.y(),
                size[2] * pixelSpacing.z());

			//precomputed values for Nearest neighbor + Linear interpolator
			double expectedDoseIncreaseXNearest[27];
			double expectedDoseIncreaseXLinear[27];

			for (int i = 0; i < 27; i++)
			{
				if (i < 9)
				{
					expectedDoseIncreaseXNearest[i] = 0.0;
                    expectedDoseIncreaseXLinear[i] = 1.41119e-005;
				}
				else if (i < 18)
				{
                    expectedDoseIncreaseXNearest[i] = 0.000141119;
                    expectedDoseIncreaseXLinear[i] = 0.000141119;
				}
				else
				{
                    expectedDoseIncreaseXNearest[i] = 0.000282239;
                    expectedDoseIncreaseXLinear[i] = 0.000279416;
				}
			}

			//TEST 1) RTDOSE_FILENAME_CONSTANT_TWO contains Dose 2.0 Gy for each pixel, so for every interpolation, 2.0 has to be the result
			std::vector<WorldCoordinate3D>::const_iterator iterCoordinates = coordinatesToCheck.cbegin();

			while (iterCoordinates != coordinatesToCheck.cend())
			{
				CHECK_EQUAL(interpolationNN->getValue(*iterCoordinates), 2.0);
				CHECK_EQUAL(interpolationLinear->getValue(*iterCoordinates), 2.0);
				++iterCoordinates;
			}

			//TEST 2) RTDOSE_FILENAME_INCREASE_X contains a Dose gradient file, correct interpolation values have been computed manually. To avoid floating point inaccuracy, CHECK_CLOSE is used
			iterCoordinates = coordinatesToCheck.cbegin();
			unsigned int index = 0;

			while (iterCoordinates != coordinatesToCheck.cend() && index < 27)
			{
				CHECK_CLOSE(interpolationNN2->getValue(*iterCoordinates), expectedDoseIncreaseXNearest[index],
				            errorConstant);
				CHECK_CLOSE(interpolationLinear2->getValue(*iterCoordinates), expectedDoseIncreaseXLinear[index],
				            errorConstant);
				++iterCoordinates;
				++index;
			}

			//TEST 3) Exception handling
			//Check that core::MappingOutOfImageException is thrown if requested position is outside image
			CHECK_THROW_EXPLICIT(interpolationNN->getValue(positionOutsideOfImageLeft),
			                     core::MappingOutsideOfImageException);
			CHECK_THROW_EXPLICIT(interpolationNN->getValue(positionOutsideOfImageRight),
			                     core::MappingOutsideOfImageException);
			CHECK_THROW_EXPLICIT(interpolationLinear->getValue(positionOutsideOfImageLeft),
			                     core::MappingOutsideOfImageException);
			CHECK_THROW_EXPLICIT(interpolationLinear->getValue(positionOutsideOfImageRight),
			                     core::MappingOutsideOfImageException);

			//Check that core::NullPointerException is thrown if Null Pointers are given to interpolator
			CHECK_THROW_EXPLICIT(interpolationNullLinear->setAccessorPointer(doseAccessorNull),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(interpolationNullNN->setAccessorPointer(doseAccessorNull),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(interpolationNullLinear->getValue(coordinatesToCheck.front()),
			                     core::NullPointerException);
			CHECK_THROW_EXPLICIT(interpolationNullNN->getValue(coordinatesToCheck.front()),
			                     core::NullPointerException);

			//Check that no exception is thrown otherwise
			CHECK_NO_THROW(boost::make_shared<NearestNeighborInterpolation>());
			CHECK_NO_THROW(boost::make_shared<LinearInterpolation>());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}
}
