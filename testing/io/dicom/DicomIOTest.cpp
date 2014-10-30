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

#include <iomanip>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomIODDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief DicomIOTest - test the IO for dicom data
		1) test dicom dose import if geometric info was set correctly
		2) test dicom dose import accessing dose data and converting
		3) check if dicom tags are correctly read
		4) test structure import

		WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int DicomIOTest(int argc, char* argv[])
		{
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: structure file name
			//           2: dose1 file name
			//           3: dose2 file name
			//           4: dose3 file name
			//           5: dose4 file name
			//           6: dose5 file name

			std::string RTSTRUCT_FILENAME;
			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;
			std::string RTDOSE3_FILENAME;
			std::string RTDOSE4_FILENAME;
			std::string RTDOSE5_FILENAME;

			if (argc > 1)
			{
				RTSTRUCT_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTDOSE2_FILENAME = argv[3];
			}

			if (argc > 4)
			{
				RTDOSE3_FILENAME = argv[4];
			}

			if (argc > 5)
			{
				RTDOSE4_FILENAME = argv[5];
			}
			if(argc > 6)
			{
				RTDOSE5_FILENAME = argv[6];
			}


			OFCondition status;
			DcmFileFormat fileformat;

			/* read dicom-rt dose */
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//1) test dicom dose import if geometric info was set correctly
			core::GeometricInfo geoInfo = doseAccessor1->getGeometricInfo();
			CHECK_EQUAL(45, geoInfo.getNumRows());
			CHECK_EQUAL(67, geoInfo.getNumColumns());
			CHECK_EQUAL(64, geoInfo.getNumSlices());
			CHECK_EQUAL(OrientationMatrix(), geoInfo.getOrientationMatrix());

			SpacingVectorType3D pixelSpacing(5, 5, 5);
			CHECK_EQUAL(pixelSpacing, geoInfo.getSpacing());

			WorldCoordinate3D imagePositionPatient(-170.556642, -273.431642, -674);
			CHECK_EQUAL(imagePositionPatient, geoInfo.getImagePositionPatient());

			//test geometric info of an inhomogeneous dose grid
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator5(RTDOSE5_FILENAME.c_str());
			DoseAccessorPointer doseAccessor5(doseAccessorGenerator5.generateDoseAccessor());

			SpacingVectorType3D pixelSpacing5(2,3,2);
			CHECK_EQUAL(pixelSpacing5, doseAccessor5->getGeometricInfo().getSpacing());

			const VoxelGridID start = 0;
			const VoxelGridIndex3D start3D(0);

			VoxelGridID end, inbetween;
			VoxelGridIndex3D end3D, inbetween3D;

			//2) test dicom dose import accessing dose data and converting

			CHECK_EQUAL(2, doseAccessor1->getDoseAt(start));
			CHECK_EQUAL(2, doseAccessor1-> getDoseAt(start3D));
			CHECK_EQUAL(doseAccessor1->getDoseAt(start), doseAccessor1-> getDoseAt(start3D));

			inbetween = int(std::floor(doseAccessor1->getGridSize() / 2.0));
			doseAccessor1->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(2, doseAccessor1->getDoseAt(inbetween));
			CHECK_EQUAL(2, doseAccessor1-> getDoseAt(inbetween3D));
			CHECK_EQUAL(doseAccessor1->getDoseAt(inbetween), doseAccessor1-> getDoseAt(inbetween3D));

			end = doseAccessor1->getGridSize() - 1;
			doseAccessor1->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(2, doseAccessor1->getDoseAt(end));
			CHECK_EQUAL(2, doseAccessor1-> getDoseAt(end3D));
			CHECK_EQUAL(doseAccessor1->getDoseAt(end), doseAccessor1-> getDoseAt(end3D));

			::DRTDoseIOD rtdose2;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			//2) test dicom dose import accessing dose data and converting

			CHECK_EQUAL(50, doseAccessor2->getDoseAt(start));
			CHECK_EQUAL(50, doseAccessor2-> getDoseAt(start3D));
			CHECK_EQUAL(doseAccessor2->getDoseAt(start), doseAccessor2-> getDoseAt(start3D));

			inbetween = int(std::floor(doseAccessor2->getGridSize() / 2.0));
			doseAccessor2->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(50, doseAccessor2->getDoseAt(inbetween));
			CHECK_EQUAL(50, doseAccessor2-> getDoseAt(inbetween3D));
			CHECK_EQUAL(doseAccessor2->getDoseAt(inbetween), doseAccessor2-> getDoseAt(inbetween3D));

			end = doseAccessor2->getGridSize() - 1;
			doseAccessor2->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(50, doseAccessor2->getDoseAt(end));
			CHECK_EQUAL(50, doseAccessor2-> getDoseAt(end3D));
			CHECK_EQUAL(doseAccessor2->getDoseAt(end), doseAccessor2-> getDoseAt(end3D));


			::DRTDoseIOD rtdose3;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator3(RTDOSE3_FILENAME.c_str());
			DoseAccessorPointer doseAccessor3(doseAccessorGenerator3.generateDoseAccessor());

			//2) test dicom dose import accessing dose data and converting

			CHECK_EQUAL(0, doseAccessor3->getDoseAt(start));
			CHECK_EQUAL(0, doseAccessor3-> getDoseAt(start3D));
			CHECK_EQUAL(doseAccessor3->getDoseAt(start), doseAccessor3-> getDoseAt(start3D));

			inbetween = int(std::floor(doseAccessor3->getGridSize() / 2.0));
			doseAccessor3->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(0, doseAccessor3->getDoseAt(inbetween));
			CHECK_EQUAL(0, doseAccessor3-> getDoseAt(inbetween3D));
			CHECK_EQUAL(doseAccessor3->getDoseAt(inbetween), doseAccessor3-> getDoseAt(inbetween3D));

			end = doseAccessor3->getGridSize() - 1;
			doseAccessor3->getGeometricInfo().convert(end, end3D);

			CHECK_CLOSE(1.46425, doseAccessor3->getDoseAt(end), errorConstant);
			CHECK_CLOSE(1.46425, doseAccessor3-> getDoseAt(end3D), errorConstant);
			CHECK_EQUAL(doseAccessor3->getDoseAt(end), doseAccessor3-> getDoseAt(end3D));

			/* structure set */
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();
			//4) test structure import
			CHECK_EQUAL(10, rtStructureSet->getNumberOfStructures());

			CHECK_EQUAL("Aussenkontur", (rtStructureSet->getStructure(0))->getLabel());
			CHECK_EQUAL("Rueckenmark", (rtStructureSet->getStructure(1))->getLabel());
			CHECK_EQUAL("Niere re.", (rtStructureSet->getStructure(2))->getLabel());
			CHECK_EQUAL("Niere li.", (rtStructureSet->getStructure(3))->getLabel());
			CHECK_EQUAL("Magen/DD", (rtStructureSet->getStructure(4))->getLabel());
			CHECK_EQUAL("Leber", (rtStructureSet->getStructure(5))->getLabel());
			CHECK_EQUAL("Darm", (rtStructureSet->getStructure(6))->getLabel());
			CHECK_EQUAL("Ref.Pkt.", (rtStructureSet->getStructure(7))->getLabel());
			CHECK_EQUAL("PTV", (rtStructureSet->getStructure(8))->getLabel());
			CHECK_EQUAL("Boost", (rtStructureSet->getStructure(9))->getLabel());


			//4) Test other tags (GridFrameOffsetVector(3004,000c) and PixelSpacingBetweenSlices(0018, 0088)) to compute dose slicing
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator4(RTDOSE4_FILENAME.c_str());
			DoseAccessorPointer doseAccessor4(doseAccessorGenerator4.generateDoseAccessor());
			rttb::SpacingVectorType3D spacingVec  = doseAccessor4->getGeometricInfo().getSpacing();
			CHECK_EQUAL(2.5, spacingVec.x());
			CHECK_EQUAL(2.5, spacingVec.y());
			CHECK_EQUAL(3, spacingVec.z());


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

