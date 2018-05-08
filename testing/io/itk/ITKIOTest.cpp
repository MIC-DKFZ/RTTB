// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author $Author$ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbITKImageAccessorConverter.h"
#include "rttbITKImageAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbITKImageFileMaskAccessorGenerator.h"
#include "rttbITKImageAccessor.h"
#include "rttbITKImageMaskAccessor.h"
#include "rttbInvalidDoseException.h"

#include "itkImageFileReader.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief ITKIOTest - test the IO for ITK data
				1) test ITK dose import if geometric info was set correctly
				2) test ITKImageFileDoseAccessorGenerator accessing dose data and converting (*.mhd file)
				3) test ITKImageDoseAccessorGenerator accessing dose data and converting (*.mhd file)

			WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int ITKIOTest(int argc, char* argv[])
		{
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;
      typedef core::MaskAccessorInterface::Pointer MaskAccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			// 1: mhd file name
      // 2: mask file name

			std::string RTDOSE_FILENAME, VOXELIZEDMASK_FILENAME;

			if (argc > 2)
			{
				RTDOSE_FILENAME = argv[1];
        VOXELIZEDMASK_FILENAME = argv[2];
			}

			/* read dose in *.mhd file */
			io::itk::ITKImageFileAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//1) test ITK dose import if geometric info was set correctly
			core::GeometricInfo geoInfo = doseAccessor1->getGeometricInfo();
			CHECK_EQUAL(25, geoInfo.getNumRows());
			CHECK_EQUAL(25, geoInfo.getNumColumns());
			CHECK_EQUAL(15, geoInfo.getNumSlices());
			//orientation matrix equals identity matrix
			CHECK_EQUAL(OrientationMatrix(), geoInfo.getOrientationMatrix());

			const VoxelGridID start = 0;
			const VoxelGridIndex3D start3D(0);

			VoxelGridID end, inbetween, inbetween2, outside;
			VoxelGridIndex3D end3D, inbetween3D, inbetween23D, outside3D;

			//2) test ITK dose import accessing dose data and converting (*.mhd file)
			CHECK(doseAccessor1->getGeometricInfo().validID(start));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(start3D));

			CHECK(!(doseAccessor1->getGeometricInfo().validID(doseAccessor1->getGridSize())));
			CHECK(!(doseAccessor1->getGeometricInfo().validIndex(VoxelGridIndex3D(
			            doseAccessor1->getGridSize()))));

			CHECK_EQUAL(0, doseAccessor1->getValueAt(start));
			CHECK_EQUAL(doseAccessor1->getValueAt(start), doseAccessor1-> getValueAt(start3D));
      CHECK_NO_THROW(doseAccessor1->getUID());

			inbetween = 4039;
			doseAccessor1->getGeometricInfo().convert(inbetween, inbetween3D);
			CHECK(doseAccessor1->getGeometricInfo().validID(inbetween));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(inbetween3D));

			CHECK_EQUAL(162.0, doseAccessor1->getValueAt(inbetween));
			CHECK_EQUAL(doseAccessor1->getValueAt(inbetween), doseAccessor1-> getValueAt(inbetween3D));

            inbetween2 = 6086;
			doseAccessor1->getGeometricInfo().convert(inbetween2, inbetween23D);
			CHECK(doseAccessor1->getGeometricInfo().validID(inbetween2));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(inbetween23D));
			CHECK_EQUAL(7.0, doseAccessor1->getValueAt(inbetween2));
			CHECK_EQUAL(doseAccessor1->getValueAt(inbetween2), doseAccessor1-> getValueAt(inbetween23D));

			end = doseAccessor1->getGridSize() - 1;
      outside = end + 1;
			doseAccessor1->getGeometricInfo().convert(end, end3D);
      outside3D = VoxelGridIndex3D(end3D.x() + 2, end3D.y(), end3D.z());
			CHECK(doseAccessor1->getGeometricInfo().validID(end));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(end3D));

			CHECK_EQUAL(0, doseAccessor1->getValueAt(end));
			CHECK_EQUAL(doseAccessor1->getValueAt(end), doseAccessor1-> getValueAt(end3D));

      CHECK_EQUAL(-1, doseAccessor1->getValueAt(outside));
      CHECK_EQUAL(-1, doseAccessor1->getValueAt(outside3D));

			typedef itk::Image< DoseTypeGy, 3 >         DoseImageType;
			typedef itk::ImageFileReader<DoseImageType> ReaderType;

			ReaderType::Pointer reader = ReaderType::New();

			reader->SetFileName(RTDOSE_FILENAME);
			//important to update the reader (won't work without)
			reader->Update();
			io::itk::ITKImageAccessorGenerator doseAccessorGenerator2(reader->GetOutput());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			//3) test ITK dose import accessing dose data and converting (ITKImageDoseAccessor)
			CHECK(doseAccessor2->getGeometricInfo().validID(start));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(start3D));

			CHECK(!(doseAccessor2->getGeometricInfo().validID(doseAccessor2->getGridSize())));
			CHECK(!(doseAccessor2->getGeometricInfo().validIndex(VoxelGridIndex3D(
			            doseAccessor2->getGridSize()))));

			CHECK_EQUAL(0, doseAccessor2->getValueAt(start));
			CHECK_EQUAL(doseAccessor2->getValueAt(start), doseAccessor2->getValueAt(start3D));

			CHECK(doseAccessor2->getGeometricInfo().validID(inbetween));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(inbetween3D));

			CHECK_EQUAL(162.0, doseAccessor2->getValueAt(inbetween));
			CHECK_EQUAL(doseAccessor2->getValueAt(inbetween), doseAccessor2->getValueAt(inbetween3D));

			CHECK(doseAccessor2->getGeometricInfo().validID(inbetween2));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(inbetween23D));

			CHECK_EQUAL(7.0, doseAccessor2->getValueAt(inbetween2));
			CHECK_EQUAL(doseAccessor2->getValueAt(inbetween2), doseAccessor2->getValueAt(inbetween23D));

			end = doseAccessor2->getGridSize() - 1;
			doseAccessor2->getGeometricInfo().convert(end, end3D);
			CHECK(doseAccessor2->getGeometricInfo().validID(end));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(end3D));

			CHECK_EQUAL(0, doseAccessor2->getValueAt(end));
			CHECK_EQUAL(doseAccessor2->getValueAt(end), doseAccessor2-> getValueAt(end3D));

      /* test ITKImageAccessor*/
      typedef itk::Image< DoseTypeGy, 3 >         DoseImageType;

      DoseImageType::Pointer invalidDose = DoseImageType::New();
      CHECK_THROW_EXPLICIT(io::itk::ITKImageAccessor(invalidDose.GetPointer()), core::InvalidDoseException);

      /* test ITKImageMaskAccessor*/
      CHECK_THROW_EXPLICIT(io::itk::ITKImageMaskAccessor(invalidDose.GetPointer()), core::InvalidDoseException);

      io::itk::ITKImageFileMaskAccessorGenerator maskAccessorGenerator(VOXELIZEDMASK_FILENAME.c_str());
      MaskAccessorPointer maskAccessor(maskAccessorGenerator.generateMaskAccessor());

      auto imageSize = maskAccessor->getGeometricInfo().getImageSize();
      end = imageSize.x()*imageSize.y()*imageSize.z() - 1;
      outside = end + 1;

      maskAccessor->getGeometricInfo().convert(end, end3D);
      outside3D = VoxelGridIndex3D(end3D.x() + 2, end3D.y(), end3D.z());

      inbetween3D = VoxelGridIndex3D(139, 61, 57);
      maskAccessor->getGeometricInfo().convert(inbetween3D, inbetween);

      core::MaskVoxel aVoxel(0);
      CHECK_EQUAL(maskAccessor->getMaskAt(start, aVoxel), true);
      CHECK_EQUAL(aVoxel.getRelevantVolumeFraction(), 0.0);
      CHECK_EQUAL(maskAccessor->getMaskAt(end, aVoxel), true);
      CHECK_EQUAL(aVoxel.getRelevantVolumeFraction(), 0.0);
      CHECK_EQUAL(maskAccessor->getMaskAt(end3D, aVoxel), true);
      CHECK_EQUAL(aVoxel.getRelevantVolumeFraction(), 0.0);
      CHECK_EQUAL(maskAccessor->getMaskAt(outside, aVoxel), false);
      CHECK_EQUAL(maskAccessor->getMaskAt(outside3D, aVoxel), false);
      CHECK_EQUAL(maskAccessor->getMaskAt(inbetween, aVoxel), true);
      CHECK_EQUAL(aVoxel.getRelevantVolumeFraction(), 1.0);
      CHECK_EQUAL(maskAccessor->getMaskAt(inbetween3D, aVoxel), true);
      CHECK_EQUAL(aVoxel.getRelevantVolumeFraction(), 1.0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

