// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// (c) Copyright 2007, DKFZ, Heidelberg, Germany
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR
// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author zhangl (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/trunk/testing/core/DVHCalculatorTest.cpp $
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbITKImageDoseAccessor.h"
#include "rttbITKImageFileDoseAccessorGenerator.h"
#include "rttbITKImageDoseAccessorGenerator.h"


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

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			// 1: mhd file name

			std::string RTDOSE_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			/* read dose in *.mhd file */
			io::itk::ITKImageFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//1) test ITK dose import if geometric info was set correctly
			core::GeometricInfo geoInfo = doseAccessor1->getGeometricInfo();
			CHECK_EQUAL(100, geoInfo.getNumRows());
			CHECK_EQUAL(100, geoInfo.getNumColumns());
			CHECK_EQUAL(60, geoInfo.getNumSlices());
			//orientation matrix equals identity matrix
			CHECK_EQUAL(OrientationMatrix(), geoInfo.getOrientationMatrix());

			const VoxelGridID start = 0;
			const VoxelGridIndex3D start3D(0);

			VoxelGridID end, inbetween, inbetween2;
			VoxelGridIndex3D end3D, inbetween3D, inbetween23D;

			//2) test ITK dose import accessing dose data and converting (*.mhd file)
			CHECK(doseAccessor1->getGeometricInfo().validID(start));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(start3D));

			CHECK(!(doseAccessor1->getGeometricInfo().validID(doseAccessor1->getGridSize())));
			CHECK(!(doseAccessor1->getGeometricInfo().validIndex(VoxelGridIndex3D(
			            doseAccessor1->getGridSize()))));

			CHECK_EQUAL(0, doseAccessor1->getDoseAt(start));
			CHECK_EQUAL(doseAccessor1->getDoseAt(start), doseAccessor1-> getDoseAt(start3D));

			inbetween = 204837;
			doseAccessor1->getGeometricInfo().convert(inbetween, inbetween3D);
			CHECK(doseAccessor1->getGeometricInfo().validID(inbetween));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(inbetween3D));

			CHECK_EQUAL(242.0, doseAccessor1->getDoseAt(inbetween));
			CHECK_EQUAL(doseAccessor1->getDoseAt(inbetween), doseAccessor1-> getDoseAt(inbetween3D));

			inbetween2 = 283742;
			doseAccessor1->getGeometricInfo().convert(inbetween2, inbetween23D);
			CHECK(doseAccessor1->getGeometricInfo().validID(inbetween2));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(inbetween23D));
			CHECK_EQUAL(111.0, doseAccessor1->getDoseAt(inbetween2));
			CHECK_EQUAL(doseAccessor1->getDoseAt(inbetween2), doseAccessor1-> getDoseAt(inbetween23D));

			end = doseAccessor1->getGridSize() - 1;
			doseAccessor1->getGeometricInfo().convert(end, end3D);
			CHECK(doseAccessor1->getGeometricInfo().validID(end));
			CHECK(doseAccessor1->getGeometricInfo().validIndex(end3D));

			CHECK_EQUAL(0, doseAccessor1->getDoseAt(end));
			CHECK_EQUAL(doseAccessor1->getDoseAt(end), doseAccessor1-> getDoseAt(end3D));

			typedef itk::Image< DoseTypeGy, 3 >         DoseImageType;
			typedef itk::ImageFileReader<DoseImageType> ReaderType;

			ReaderType::Pointer reader = ReaderType::New();

			reader->SetFileName(RTDOSE_FILENAME);
			//important to update the reader (won't work without)
			reader->Update();
			io::itk::ITKImageDoseAccessorGenerator doseAccessorGenerator2(reader->GetOutput());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			//3) test ITK dose import accessing dose data and converting (ITKImageDoseAccessor)
			CHECK(doseAccessor2->getGeometricInfo().validID(start));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(start3D));

			CHECK(!(doseAccessor2->getGeometricInfo().validID(doseAccessor2->getGridSize())));
			CHECK(!(doseAccessor2->getGeometricInfo().validIndex(VoxelGridIndex3D(
			            doseAccessor2->getGridSize()))));

			CHECK_EQUAL(0, doseAccessor2->getDoseAt(start));
			CHECK_EQUAL(doseAccessor2->getDoseAt(start), doseAccessor2->getDoseAt(start3D));

			CHECK(doseAccessor2->getGeometricInfo().validID(inbetween));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(inbetween3D));

			CHECK_EQUAL(242.0, doseAccessor2->getDoseAt(inbetween));
			CHECK_EQUAL(doseAccessor2->getDoseAt(inbetween), doseAccessor2->getDoseAt(inbetween3D));

			CHECK(doseAccessor2->getGeometricInfo().validID(inbetween2));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(inbetween23D));

			CHECK_EQUAL(111.0, doseAccessor2->getDoseAt(inbetween2));
			CHECK_EQUAL(doseAccessor2->getDoseAt(inbetween2), doseAccessor2->getDoseAt(inbetween23D));

			end = doseAccessor2->getGridSize() - 1;
			doseAccessor2->getGeometricInfo().convert(end, end3D);
			CHECK(doseAccessor2->getGeometricInfo().validID(end));
			CHECK(doseAccessor2->getGeometricInfo().validIndex(end3D));

			CHECK_EQUAL(0, doseAccessor2->getDoseAt(end));
			CHECK_EQUAL(doseAccessor2->getDoseAt(end), doseAccessor2-> getDoseAt(end3D));


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

