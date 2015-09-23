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
			boost::shared_ptr<NearestNeighborInterpolation> interpolationNN =
			    boost::shared_ptr<NearestNeighborInterpolation>(new NearestNeighborInterpolation());
			interpolationNN->setAccessorPointer(doseAccessor1);
			boost::shared_ptr<LinearInterpolation> interpolationLinear = boost::shared_ptr<LinearInterpolation>
			        (new LinearInterpolation());
			interpolationLinear->setAccessorPointer(doseAccessor1);

			//doseAccessor2 is used as dose image.
			//RTDOSE_FILENAME_INCREASE_X and RTDOSE_FILENAME_CONSTANT_TWO have the same GeometricInfo
			boost::shared_ptr<NearestNeighborInterpolation> interpolationNN2 =
			    boost::shared_ptr<NearestNeighborInterpolation>(new NearestNeighborInterpolation());
			interpolationNN2->setAccessorPointer(doseAccessor2);
			boost::shared_ptr<LinearInterpolation> interpolationLinear2 =
			    boost::shared_ptr<LinearInterpolation>(new LinearInterpolation());
			interpolationLinear2->setAccessorPointer(doseAccessor2);

			boost::shared_ptr<NearestNeighborInterpolation> interpolationNullNN =
			    boost::shared_ptr<NearestNeighborInterpolation>(new NearestNeighborInterpolation());
			boost::shared_ptr<LinearInterpolation> interpolationNullLinear =
			    boost::shared_ptr<LinearInterpolation>(new LinearInterpolation());

			rttb::WorldCoordinate3D imagePositionPatient =
			    doseAccessor1->getGeometricInfo().getImagePositionPatient();
			rttb::SpacingVectorType3D pixelSpacing = doseAccessor1->getGeometricInfo().getSpacing();
			int size[] = {doseAccessor1->getGeometricInfo().getNumColumns(), doseAccessor1->getGeometricInfo().getNumRows(), doseAccessor1->getGeometricInfo().getNumSlices()};

			//check position [0 0 0], [max/2, max/2, max/2], [max max max] and combinations (as pixel positions are given at middle, correct with PixelSpacing/2)
			//outermost position of the image max_x = (coordinatesZeroAndMaxX.at(0) + (size[0]*pixelSpacing.x()) can't be used, because this is already outside of the image, thus test max-0.001.
			std::vector<double> coordinatesZeroAndMaxX;
			coordinatesZeroAndMaxX.push_back(imagePositionPatient.x() - (pixelSpacing.x() * 0.5));
			coordinatesZeroAndMaxX.push_back(coordinatesZeroAndMaxX.at(0) + (size[0]
			                                 *pixelSpacing.x() - 0.001));
			coordinatesZeroAndMaxX.push_back(coordinatesZeroAndMaxX.at(0) + ((size[0] - 1)*pixelSpacing.x() *
			                                 0.5));
			std::vector<double> coordinatesZeroAndMaxY;
			coordinatesZeroAndMaxY.push_back(imagePositionPatient.y() - (pixelSpacing.y() * 0.5));
			coordinatesZeroAndMaxY.push_back(coordinatesZeroAndMaxY.at(0) + (size[1]
			                                 *pixelSpacing.y() - 0.001));
			coordinatesZeroAndMaxY.push_back(coordinatesZeroAndMaxY.at(0) + ((size[1] - 1)*pixelSpacing.y() *
			                                 0.5));
			std::vector<double> coordinatesZeroAndMaxZ;
			coordinatesZeroAndMaxZ.push_back(imagePositionPatient.z() - (pixelSpacing.z() * 0.5));
			coordinatesZeroAndMaxZ.push_back(coordinatesZeroAndMaxZ.at(0) + (size[2]
			                                 *pixelSpacing.z() - 0.001));
			coordinatesZeroAndMaxZ.push_back(coordinatesZeroAndMaxZ.at(0) + ((size[2] - 1)*pixelSpacing.z() *
			                                 0.5));

			std::vector<rttb::WorldCoordinate3D> coordinatesToCheck;

			for (int x = 0; x < coordinatesZeroAndMaxX.size(); x++)
			{
				for (int y = 0; y < coordinatesZeroAndMaxY.size(); y++)
				{
					for (int z = 0; z < coordinatesZeroAndMaxZ.size(); z++)
					{
						coordinatesToCheck.push_back(rttb::WorldCoordinate3D(coordinatesZeroAndMaxX.at(x),
						                             coordinatesZeroAndMaxY.at(y), coordinatesZeroAndMaxZ.at(z)));
					}
				}
			}

			rttb::WorldCoordinate3D positionOutsideOfImageLeft = imagePositionPatient - rttb::WorldCoordinate3D(
			            pixelSpacing.x() * 2.0, pixelSpacing.y() * 2.0, pixelSpacing.z() * 2.0);
			rttb::WorldCoordinate3D positionOutsideOfImageRight = imagePositionPatient +
			        rttb::WorldCoordinate3D(size[0] * pixelSpacing.x(), size[1] * pixelSpacing.y(),
			                                size[2] * pixelSpacing.z()) + rttb::WorldCoordinate3D(pixelSpacing.x() * 2.0,
			                                        pixelSpacing.y() * 2.0, pixelSpacing.z() * 2.0);

			//precomputed values for Nearest neighbor + Linear interpolator
			double expectedDoseIncreaseXNearest[27];
			double expectedDoseIncreaseXLinear[27];

			for (int i = 0; i < 27; i++)
			{
				if (i < 9)
				{
					expectedDoseIncreaseXNearest[i] = 0.0;
					expectedDoseIncreaseXLinear[i] = 0.0;
				}
				else if (i < 18)
				{
					expectedDoseIncreaseXNearest[i] = 0.00186277;
					expectedDoseIncreaseXLinear[i] = 0.00186277;
				}
				else
				{
					expectedDoseIncreaseXNearest[i] = 0.000931387;
					expectedDoseIncreaseXLinear[i] = 0.000931387;
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
			CHECK_NO_THROW(boost::shared_ptr<NearestNeighborInterpolation>(new NearestNeighborInterpolation()));
			CHECK_NO_THROW(boost::shared_ptr<LinearInterpolation>(new LinearInterpolation()));

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}
}
